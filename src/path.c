#include "path.h"

#include <math.h>

// forward declarations for heap operations.
static void heapDown( PathData *data );
static void heapUp( PathData *data );
static void heapAdd( PathData *data, int x, int y );
static int heapGet( PathData *data);
static void heapReorder( PathData *data, int offset );

//forward declarations for internal path functions
static void path_getCell( PathData *data, int *x, int *y, float *distance);
static void path_buildArrays( PathData *data );
static float path_cost(PathData *data, int x, int y);

//TODO: allow for custom function to combine maps?
PathData *path_new( Map_t *map )
{
    PathData *data = calloc(sizeof(PathData), 1);
    check_mem(data);
    int w = map->width; int h = map->height;
    data->distance = calloc(sizeof(float), w*h);
    data->aStar = calloc(sizeof(float), w*h);
    data->prev = calloc(sizeof(float), w*h);

    utarray_new( data->path, &ut_int_icd);
    utarray_reserve(data->path, 64);

    utarray_new( data->heap, &ut_int_icd);
    utarray_reserve(data->heap, 64);

    return data;
error:
    return NULL;
}

bool path_compute( PathData *data, int x0, int y0, int dx, int dy)
{
    data->x0 = x0;
    data->y0 = y0;
    data->dx = dx;
    data->dy = dy;
    int w = data->map->width;
    int h = data->map->height;

    utarray_clear(data->path);
    utarray_clear(data->heap);
    // If origin is same as destination
    if ( x0 == dx && y0 == dy) return true;
    check( x0 < w && y0 < h, "Bounds error");
    check( dx < w && dy < h, "Bound error");
    /* Initialize djikstra distance grids to zero? only needed if path is being recalc'd
     * since we initialize to zero anyways. */
    // memset(data->distance, 0, sizeof(float) * w * h);
    // memset(data->aStar, 0, sizeof(float) * w * h); 
    data->aStar[ x0 + y0 * w ] = 1.0f;
    // Add the origin to our heap.
    heapAdd( data, x0, y0 );
    path_buildArrays( data );
   
    // if distance[dx][dy] == 0, no path found. 
    if (data->distance[dx + dy * w] == 0)
        return false;

    while ( dx != x0 || dy != y0 )
    {
        int step = data->prev[dx + dy * w];
        utarray_push_back( data->path, &step );
        dx -= dirList[step][0];
        dy -= dirList[step][1];
    }
    return true;
error:
    return false;
}

bool path_isEmpty( PathData *data )
{
    return (utarray_len(data->path) == 0) ? true : false;
}

int path_length( PathData *data )
{
    return utarray_len(data->path);
}

bool path_step( PathData *data, int *x, int *y, bool recalc)
{
    int newx, newy;
    float stepCost;

    if( path_isEmpty( data ) )
        return false;
    int dest = (int) *(utarray_back(data->path));
    utarray_pop_back(data->path);
    newx = data->x0 + dirList[dest][0];
    newy = data->y0 + dirList[dest][1];

    stepCost = path_cost( data, newx, newy );
    //if a cell we are pathing to becomes blocked, recalc if needed.
    if (stepCost == 0.0f)
    {
        if ( recalc == false )
            return false;
        if ( path_compute( data, data->x0, data->y0, data->dx, data->dy) == false )
            return false;
        // recursive call to continue along path.
        return path_step(data, x, y, true); 
    }

    if (x) *x = newx;
    if (y) *y = newy;
    data->x0 = newx;
    data->y0 = newy;
    return true;
}

void path_atIndex( PathData *data, int index, int *x, int *y )
{
    int cur;
    if( x ) *x = data->x0;
    if( y ) *y = data->y0;

    cur = path_length(data) - 1;
    while (index >= 0)
    {
        int dir = *((int*)utarray_eltptr(data->path, cur));
        if ( x ) *x += dirList[dir][0];
        if ( y ) *y += dirList[dir][1];

        cur--; index--;
    }
}

void path_delete( PathData *data )
{
    if (data->distance) free(data->distance);
    if (data->aStar)    free(data->aStar);
    if (data->prev)     free(data->prev);
    if (data->path)     utarray_free(data->path);
    if (data->heap)     utarray_free(data->heap);
    free(data);
}
// Start from our origin, and calculate our aStar/Distances all the way to dest.
static void path_buildArrays( PathData *data )
{
    int dx = data->dx; int dy = data->dy;
    int w = data->map->width;
    int h = data->map->height;

    while( data->distance[dx + dy * w] == 0 && utarray_len(data->heap) != 0)
    {
        int x0, y0;
        float dist;
        // pop a cell off our heap.
        path_getCell( data, &x0, &y0, &dist );
        for( int i = 0; i < NUM_DIRECTIONS; i++ )
        {
            // get coordinates of adjacent cell.
            int x1 = x0 + dirList[i][0];
            int y1 = y0 + dirList[i][1];
            check( x1 >= 0 && y1 >= 0 && x1 < w && y1 < h, "Bounds Error");

            float cost = path_cost(data, x1, y1);
            if (cost)
            {
            //  if we add diagonal cost, visited = distance + cost * (isDiagonal(i) ? dCost: 1.0f);
                float totalCost = dist + cost;
                float prevCost = data->distance[ x1 + y1 * w ];
                // If we haven't visited the cell, add it to heap.
                if( prevCost == 0 )
                {
                    int offset = x1 + y1 * w;
                    /* A* heuristic; libtcod remaining distance function */
                    //TODO Implement different heuristic without sqrt func.
                    float heur = (float) sqrt((x1-dx)*(x1-dx) + (y1-dy)*(y1-dy));

                    data->distance[offset] = totalCost;
                    data->aStar[offset] = totalCost + heur;
                    data->prev[offset] = i;
                    heapAdd( data, x1, y1 );
                }
                // If we find a better path to a cell in heap.
                else if (prevCost > totalCost)
                {
                    int offset = x1 + y1 * w;
                    data->distance[offset] = totalCost;
                    data->aStar[offset] -= (prevCost - totalCost);
                    data->prev[offset] = i;
                    // reorder our heap, this is slow.
                    heapReorder( data, offset );
                }
            }
        }
    }
error:
    return;
}

static void path_getCell( PathData *data, int *x, int *y, float *distance)
{
    int offset = heapGet( data );
    int w = data->map->width;
    *x = offset % w;
    *y = offset / w;
    *distance = data->distance[offset];
}

//TODO Expand this to allow terrain type modifiers?
static float path_cost(PathData *data, int x, int y)
{
    return map_pathableCell(data->map, x, y) ? 1.0f: 0.0f;
}

/*
 * Prio-Queue implemented as binary min-heap
 */
static void heapDown( PathData *data )
{
    int cur = 0;
    int len = utarray_len( data->heap ) - 1;
    int child = 1;

    int *arr = (int *)utarray_front(data->heap);
    while (child <= len)
    {
        int swap = cur;
        int curOff = arr[cur];
        float curDist = data->aStar[curOff];
        float swapValue = curDist;

        int childOff = arr[child];
        float childDist = data->aStar[childOff];
        if( childDist < curDist )
        {
            swap = child;
            swapValue = childDist;
        }
        if ( child < len )
        {
            int child2_off = arr[child+1];
            float child2_dist = data->aStar[child2_off];
            if( swapValue > child2_dist)
            {
                swap = child + 1;
                swapValue = child2_dist;
            }
        }
        if( swap != cur )
        {
            int tmp = arr[swap];
            arr[swap] = arr[cur];
            arr[cur] = tmp;
            cur = swap;
        } else return;
        child = cur * 2 + 1;
    }
}

static void heapUp( PathData *data )
{
    int end = utarray_len(data->heap) - 1;
    int child = end;
    int *arr = (int *)utarray_front(data->heap);

    while( child > 0 )
    {
        int childOff = arr[child];
        float childDist = data->aStar[childOff];
        int parent = (child - 1) / 2;
        int parentOff = arr[parent];
        float parentDist = data->aStar[parentOff];
        if (parentDist > childDist )
        {
            int tmp = arr[child];
            arr[child] = arr[parent];
            arr[parent] = tmp;
            child = parent;
        } 
        else return;
    }
}

static void heapAdd( PathData *data, int x, int y )
{
    int offset = x + y * data->map->width;
    utarray_push_back(data->heap, &offset);
    heapUp(data);
}

static int heapGet( PathData *data)
{
    int *arr = (int*) utarray_front(data->heap);
    int end = utarray_len(data->heap);
    int offset = (int) arr[0];
    arr[0] = arr[end];
    utarray_pop_back(data->heap);
    heapDown(data);

    return offset;
}

/* To change aStar of cell already in heap. */
static void heapReorder( PathData *data, int offset )
{
    int *arr = (int*)utarray_front(data->heap);
    int *end = (int*)utarray_back(data->heap);
    int *cur = arr;

    int offsetIndex;
    float value;
    int i = 0;
    int heapSize = utarray_len(data->heap);
    
    while( cur != end )
    {
        if (*cur == offset)
            break;
        cur++;
        i++;
    }
    if ( cur == end ) return;
    offsetIndex = arr[i];
    value = data->aStar[offsetIndex];
    if ( i > 0 )
    {
        // Compare to parent node.
        int parent = (i-1)/2;
        int parentOff = arr[parent];
        float parentValue = data->aStar[parentOff];
        if( value < parentValue )
        {
            // Smaller than parent, move up heap.
            while (i > 0 && value < parentValue)
            {
                arr[parent] = offsetIndex;
                arr[i] = parentOff;
                i = parent;
                if ( i > 0 )
                {
                    parent = (i-1)/2;
                    parentOff = arr[parent];
                    parentValue = data->aStar[parentOff];
                }
            }
            return;
        }
    }
    // Compare to sons.
    while ( (i*2+1) < heapSize)
    {
        int child = i*2+1;
        int childOff = arr[child];
        int swap = i;
        float swapValue = value;
        if(data->aStar[childOff] < value)
        {
            swap = child;
            swapValue = data->aStar[childOff];
        }
        int childTwo = child+1;
        if( childTwo < heapSize )
        {
            int childTwoOff = arr[childTwo];
            if ( data->aStar[childTwoOff] < swapValue)
            {
                // Swap with son 2.
                swap = childTwo;
            }
        }
        if ( swap != i)
        {
            // swap is bigger, move it down.
            int tmp = arr[swap];
            arr[swap] = arr[i];
            arr[i] = tmp;
            i = swap;
        } else
            return;
    }
}




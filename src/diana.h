#ifndef __DIANA_H__
#define __DIANA_H__

#define DIANA_VERSION "0.0.3.0"

#ifndef DL_COMPUTE
#define DL_COMPUTE 1
#endif

#include <stddef.h>
enum {
	DL_ERROR_NONE,
	DL_ERROR_OUT_OF_MEMORY,
	DL_ERROR_INVALID_VALUE,
	DL_ERROR_INVALID_OPERATION,
	DL_ERROR_FULL_COMPONENT
};

// component flags
#define DL_COMPONENT_INDEXED_BIT  1
#define DL_COMPONENT_MULTIPLE_BIT 2
#define DL_COMPONENT_LIMITED_BIT  4

#define DL_COMPONENT_FLAG_INLINE     0
#define DL_COMPONENT_FLAG_INDEXED    DL_COMPONENT_INDEXED_BIT
#define DL_COMPONENT_FLAG_MULTIPLE   (DL_COMPONENT_INDEXED_BIT | DL_COMPONENT_MULTIPLE_BIT)
#define DL_COMPONENT_FLAG_LIMITED(X) (DL_COMPONENT_INDEXED_BIT | DL_COMPONENT_LIMITED_BIT | ((X) << 3))

// system flags
#define DL_SYSTEM_PASSIVE_BIT 1

#define DL_SYSTEM_FLAG_NORMAL  0
#define DL_SYSTEM_FLAG_PASSIVE DL_SYSTEM_PASSIVE_BIT

// manager flags
#define DL_MANAGER_FLAG_NORMAL  0

// entity signal
enum {
	DL_ENTITY_ADDED,
	DL_ENTITY_ENABLED,
	DL_ENTITY_DISABLED,
	DL_ENTITY_DELETED
};

// ============================================================================
// DIANA
struct diana;

int allocate_diana(void *(*malloc)(size_t), void (*free)(void *), struct diana **);

int diana_free(struct diana *);

// ============================================================================
// INITIALIZATION TIME
int diana_initialize(struct diana *);

// ============================================================================
// component
int diana_createComponent(
	struct diana *diana,
	const char *name,
 	size_t size,
 	unsigned int flags,
	unsigned int * component_ptr
);

#if DL_COMPUTE
int diana_componentCompute(struct diana *diana, unsigned int component, void (*compute)(struct diana *, void *, unsigned int entity, unsigned int index, void *), void *userData);
#endif

// ============================================================================
// system
int diana_createSystem(
	struct diana *diana,
	const char *name,
	void (*starting)(struct diana *, void *),
	void (*process)(struct diana *, void *, unsigned int, float),
	void (*ending)(struct diana *, void *),
	void (*subscribed)(struct diana *, void *, unsigned int),
	void (*unsubscribed)(struct diana *, void *, unsigned int),
	void *userData,
	unsigned int flags,
	unsigned int * system_ptr
);

int diana_watch(struct diana *diana, unsigned int system, unsigned int component);

int diana_exclude(struct diana *diana, unsigned int system, unsigned int component);

// ============================================================================
// manager
int diana_createManager(
	struct diana *diana,
	const char *name,
	void (*added)(struct diana *, void *, unsigned int),
	void (*enabled)(struct diana *, void *, unsigned int),
	void (*disabled)(struct diana *, void *, unsigned int),
	void (*deleted)(struct diana *, void *, unsigned int),
	void *userData,
	unsigned int flags,
	unsigned int * manager_ptr
);

// ============================================================================
// RUNTIME
int diana_process(struct diana *, float delta);

int diana_processSystem(struct diana *, unsigned int system, float delta);

// ============================================================================
// entity
int diana_spawn(struct diana *diana, unsigned int * entity_ptr);

int diana_clone(struct diana *diana, unsigned int parentEntity, unsigned int * entity_ptr);

int diana_signal(struct diana *diana, unsigned int entity, unsigned int signal);

// single
int diana_setComponent(struct diana *diana, unsigned int entity, unsigned int component, const void * data);

int diana_getComponent(struct diana *diana, unsigned int entity, unsigned int component, void ** data_ptr);

#if DL_COMPUTE
int diana_dirtyComponent(struct diana *diana, unsigned int entity, unsigned int component);
#endif

int diana_removeComponent(struct diana *diana, unsigned int entity, unsigned int component);

// multiple
int diana_getComponentCount(struct diana *diana, unsigned int entity, unsigned int component, unsigned int * count_ptr);

int diana_appendComponent(struct diana *diana, unsigned int entity, unsigned int component, const void * data);

int diana_removeComponents(struct diana *diana, unsigned int entity, unsigned int component);

// low level
int diana_setComponentI(struct diana *diana, unsigned int entity, unsigned int component, unsigned int i, const void * data);

int diana_getComponentI(struct diana *diana, unsigned int entity, unsigned int component, unsigned int i, void ** ptr);

int diana_removeComponentI(struct diana *diana, unsigned int entity, unsigned int component, unsigned int i);

#endif

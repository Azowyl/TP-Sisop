#include "decls.h"
#include "sched.h"

#define MAX_TASK 10
#define INT_FLAG 0x0200
#define KSEG_CODE 8

static struct Task Tasks[MAX_TASK];
static struct Task *current;

static task_kill() {
	current->status = FREE;
	sched(NULL);
}

void sched_init() {
	current = &(Tasks[0]);
	current->status = RUNNING;
}

void spawn(void (*entry)(void)) {
	// Encontrar en el arreglo global 
	// Tasks, una entrada con estado FREE
	int i = 0;
	while ( Tasks[i].status != FREE ) {
		i++;
	}
	
	// Cambiar su status a READY
	Tasks[i].status = READY;

	// Frame apunta a tope de pila
	uint32_t initial_stack_pos = STACK_SIZE - sizeof(struct TaskFrame) - 1;
	Tasks[i].frame = ((struct TaskFrame *) &(Tasks[i].stack[initial_stack_pos]));

	// Inicializar registros
	(Tasks[i].frame)->edi = 0;
	(Tasks[i].frame)->esi = 0;
	(Tasks[i].frame)->ebp = 0;
	(Tasks[i].frame)->esp = 0;
	(Tasks[i].frame)->ebx = 0;
	(Tasks[i].frame)->edx = 0;
	(Tasks[i].frame)->ecx = 0;
	(Tasks[i].frame)->eax = 0;
	(Tasks[i].frame)->eip = (uint32_t) entry;
	(Tasks[i].frame)->cs = KSEG_CODE;
	(Tasks[i].frame)->padding = 0;
	(Tasks[i].frame)->eflags = INT_FLAG;
	(Tasks[i].frame)->kill_fn = task_kill;
}

void sched(struct TaskFrame *tf) {
	struct Task *new = 0;
	struct Task *old = current;

	// Encontrar en el arreglo 
	// la tarea en ejecución
	int i = 0;
	while ( &(Tasks[i]) != current ) {
		i++;
	}

	i++;	// Siguiente tarea

	// Buscar siguiente tarea en READY
	while ( Tasks[i].status != READY ) {
		i++;
		if (i >= MAX_TASK) {	// Reinicio cola
			i = 0;
		}
	}

	new = &(Tasks[i]);

	old->status = READY;
	old->frame = tf;

	current = new;
	current->status = RUNNING;

	asm("movl %0, %%esp\n"
    "popa\n"
    "iret\n"
    :
    : "g"(current->frame)
    : "memory");
}

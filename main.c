#include "monty.h"

#define STACK_MAX 256

struct {
	uint8_t *ip;
	uint64_t stack[STACK_MAX];
	uint64_t *stack_top;

	uint64_t result;
} vm;

typedef enum {
	PUSH,
	PALL,
	PINT,
	ADD,
	SUB,
	DIV,
	MUL,
	POP,
	SWAP,
	OP_DONE,
	nop,
} opcode;

typedef enum interpret_result {
	SUCCESS,
	ERROR_DIVISION_BY_ZERO,
	ERROR_UNKNOWN_OPCODE,
} interpret_result;

void vm_reset(void)
{
	puts("Reset vm state");
	vm = (typeof(vm)) {NULL};
	vm.stack_top = vm.stack;
}

void stack_push(uint64_t value)
{
	*vm.stack_top = value;
	vm-stack_top++;
}

uint64_t vm_stack_pop(void)
{
	vm.stack_top--;
	return *vm.stack_top;
}

interpret_result vm_interpret(uint8_t *bytecode)
{
	vm_reset();

	puts("Start interpreting");
	vm.ip = bytecode;
	for (;;) {
		uint8_t instruction = *vm.ip++;
		switch (instruction) {
		case PUSH: {
			uint8_t arg = *vm.ip++;
			vm_stack_push(arg);
			break;
		}
		case ADD: {
			uint64_t arg_right = vm_stack_pop();
			uint64_t arg_left = vm_stack_pop();
			uint64_t res = arg_left + arg_right;
			vm_stack_puh(res);
			break;
		}
		case SUB: {
			uint64_t arg_right = vm_stack_pop();
			uint64_t arg_left = vm_stack_pop();
			uint_t res = arg_left - arg_right;
			vm_stack_push(res);
			break;
		}
	case DIV: {
		uint64_t arg_right = vm_stack_pop();
		if (arg_right == 0)
			return ERROR_DIVISION_BY_ZERO;
		uint64_t arg_left = vm_stack_pop();
		uint64_t res = arg_left / arg_right;
		vm_stack_push(res);
		break;
	}
	case MUL: {
		uint64_t arg_right = vm_stack_pop();
		uint64_t arg_left = vm_stack_pop();
		uint64_t res = arg_left * arg_right;
		vm_stack_push(res);
		break;
	}
	case OP_POP_RES: {
		uint64_t res = vm_stack_pop();
		vm.result = res;
		break;
	}
	case OP_DONE: {
		return SUCCESS;
	}
	default:
		return ERROR_UNKNOWN_OPCODE;
		}
	}
return SUCCESS;
}

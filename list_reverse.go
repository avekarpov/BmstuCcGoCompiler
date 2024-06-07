package main

type Node struct {
	next int
	val int
}

func main() int {
	// init list memory
	const end int = -1
	const size int = 10

	head := 0
	var memory[size] Node

	for i := head; i < size - 1; i = i + 1 {
		memory[i].next = i + 1
		memory[i].val = (i + 1) * 10
		printf("{next: %d, val: %d}, ", memory[i].next, memory[i].val)
	}
	memory[size - 1].next = end
	memory[size - 1].val = (size + 1) * 10
	printf("{next: %d, val: %d}, ", memory[size - 1].next,  memory[size - 1].val)
	printf("\n")

	// reverse
	new_head := size - 1
	var new_memory[size] Node

	new_memory[new_head].next = end
	for i := 0; i < size - 1; i = i + 1 {
		new_memory[new_head].val = memory[head].val
		
		head = memory[head].next

		new_node := new_head - 1 // fake allocation
		new_memory[new_node].next = new_head
		new_head = new_node
	}

	memory = new_memory
	head = new_head

	for i := 0; i < size; i = i + 1 {
		printf("{next: %d, val: %d}, ", memory[i].next, memory[i].val)
	}
	printf("\n")


	return 0
}
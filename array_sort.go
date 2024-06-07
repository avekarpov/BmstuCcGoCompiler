package main

func swap(lhs *int, rhs *int) {
	tmp := *lhs
	*lhs = *rhs
	*rhs = tmp
}

func main() int {
	const size int = 10

	printf("input array[%d]: ", size)
	var array[size] int
	for i := 0; i < size; i = i + 1 {
		scanf("%d", &array[i])
	}

	for i := 0; i < size; i = i + 1 {
		for j := i + 1; j < size; j = j + 1 {
			if (array[i] < array[j]) {
				swap(&array[i], &array[j])
			}
		}
	}

	printf("sorted: ")
	for i := 0; i < size; i = i + 1 {
		printf("%d, ", array[i])
	}
	printf("\n")

	return 0
}
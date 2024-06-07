package main

func fib(i int) int {
	if (i == 0) {
		return 0
	}
	if (i == 1) {
		return 1
	}

	return fib(i - 1) + fib(i - 2)
}

func main() int {
	var i int
	printf("input number: ")
	scanf("%d", &i)
	i = fib(i)
	printf("%d\n", i)
	return 0
}
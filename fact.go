package main

func fact(i int) int {
	if (i <= 1) {
		return 1
	}

	return fact(i - 1) * i
}

func main() int {
	var i int
	printf("input number: ")
	scanf("%d", &i)
	i = fact(i)
	printf("%d\n", i)
	return 0
}
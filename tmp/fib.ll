; ModuleID = 'Id'
source_filename = "Id"

@0 = private unnamed_addr constant [15 x i8] c"input number: \00", align 1
@1 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(ptr, ...)

declare i32 @scanf(ptr, ...)

define i32 @fib(i32 %i) {
entry:
  %0 = alloca i32, align 4
  %i1 = alloca i32, align 4
  store i32 %i, ptr %i1, align 4
  %1 = load i32, ptr %i1, align 4
  %2 = icmp eq i32 0, %1
  br i1 %2, label %true_cond, label %false_cond

exit:                                             ; preds = %after_if4, %true_cond2, %true_cond
  %3 = load i32, ptr %0, align 4
  ret i32 %3

true_cond:                                        ; preds = %entry
  store i32 0, ptr %0, align 4
  br label %exit

false_cond:                                       ; preds = %entry
  br label %after_if

after_if:                                         ; preds = %false_cond
  %4 = load i32, ptr %i1, align 4
  %5 = icmp eq i32 1, %4
  br i1 %5, label %true_cond2, label %false_cond3

true_cond2:                                       ; preds = %after_if
  store i32 1, ptr %0, align 4
  br label %exit

false_cond3:                                      ; preds = %after_if
  br label %after_if4

after_if4:                                        ; preds = %false_cond3
  %6 = load i32, ptr %i1, align 4
  %7 = sub i32 %6, 1
  %8 = call i32 @fib(i32 %7)
  %9 = load i32, ptr %i1, align 4
  %10 = sub i32 %9, 2
  %11 = call i32 @fib(i32 %10)
  %12 = add i32 %8, %11
  store i32 %12, ptr %0, align 4
  br label %exit
}

define i32 @main() {
entry:
  %0 = alloca i32, align 4
  %i = alloca i32, align 4
  %1 = call i32 (ptr, ...) @printf(ptr @0)
  %2 = call i32 (ptr, ...) @scanf(ptr @1, ptr %i)
  %3 = load i32, ptr %i, align 4
  %4 = call i32 @fib(i32 %3)
  store i32 %4, ptr %i, align 4
  %5 = load i32, ptr %i, align 4
  %6 = call i32 (ptr, ...) @printf(ptr @2, i32 %5)
  store i32 0, ptr %0, align 4
  br label %exit

exit:                                             ; preds = %entry
  %7 = load i32, ptr %0, align 4
  ret i32 %7
}

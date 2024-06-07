; ModuleID = 'Id'
source_filename = "Id"

@0 = private unnamed_addr constant [15 x i8] c"input number: \00", align 1
@1 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(ptr, ...)

declare i32 @scanf(ptr, ...)

define i32 @fact(i32 %i) {
entry:
  %0 = alloca i32, align 4
  %i1 = alloca i32, align 4
  store i32 %i, ptr %i1, align 4
  %1 = load i32, ptr %i1, align 4
  %2 = icmp sle i32 %1, 1
  br i1 %2, label %true_cond, label %false_cond

exit:                                             ; preds = %after_if, %true_cond
  %3 = load i32, ptr %0, align 4
  ret i32 %3

true_cond:                                        ; preds = %entry
  store i32 1, ptr %0, align 4
  br label %exit
  br lable %after_if

false_cond:                                       ; preds = %entry
  br label %after_if

after_if:                                         ; preds = %false_cond
  %4 = load i32, ptr %i1, align 4
  %5 = sub i32 %4, 1
  %6 = call i32 @fact(i32 %5)
  %7 = load i32, ptr %i1, align 4
  %8 = mul i32 %6, %7
  store i32 %8, ptr %0, align 4
  br label %exit
}

define i32 @main() {
entry:
  %0 = alloca i32, align 4
  %i = alloca i32, align 4
  %1 = call i32 (ptr, ...) @printf(ptr @0)
  %2 = call i32 (ptr, ...) @scanf(ptr @1, ptr %i)
  %3 = load i32, ptr %i, align 4
  %4 = call i32 @fact(i32 %3)
  store i32 %4, ptr %i, align 4
  %5 = load i32, ptr %i, align 4
  %6 = call i32 (ptr, ...) @printf(ptr @2, i32 %5)
  store i32 0, ptr %0, align 4
  br label %exit

exit:                                             ; preds = %entry
  %7 = load i32, ptr %0, align 4
  ret i32 %7
}

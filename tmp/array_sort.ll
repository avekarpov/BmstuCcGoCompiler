; ModuleID = 'Id'
source_filename = "Id"

@0 = private unnamed_addr constant [18 x i8] c"input array[%d]: \00", align 1
@1 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@2 = private unnamed_addr constant [9 x i8] c"sorted: \00", align 1
@3 = private unnamed_addr constant [5 x i8] c"%d, \00", align 1
@4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(ptr, ...)

declare i32 @scanf(ptr, ...)

define void @swap(ptr %lhs, ptr %rhs) {
entry:
  %lhs1 = alloca ptr, align 8
  store ptr %lhs, ptr %lhs1, align 8
  %rhs2 = alloca ptr, align 8
  store ptr %rhs, ptr %rhs2, align 8
  %0 = load ptr, ptr %lhs1, align 8
  %1 = load i32, ptr %0, align 4
  %tmp = alloca i32, align 4
  store i32 %1, ptr %tmp, align 4
  %2 = load ptr, ptr %lhs1, align 8
  %3 = load ptr, ptr %rhs2, align 8
  %4 = load i32, ptr %3, align 4
  store i32 %4, ptr %2, align 4
  %5 = load ptr, ptr %rhs2, align 8
  %6 = load i32, ptr %tmp, align 4
  store i32 %6, ptr %5, align 4
  ret void
}

define i32 @main() {
entry:
  %0 = alloca i32, align 4
  %1 = call i32 (ptr, ...) @printf(ptr @0, i32 10)
  %array = alloca [10 x i32], align 4
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  %2 = load i32, ptr %i, align 4
  %3 = icmp ult i32 %2, 10
  br i1 %3, label %loop, label %after

exit:                                             ; preds = %after8
  %4 = load i32, ptr %0, align 4
  ret i32 %4

loop:                                             ; preds = %loop, %entry
  %5 = load i32, ptr %i, align 4
  %6 = getelementptr inbounds [10 x i32], ptr %array, i32 0, i32 %5
  %7 = call i32 (ptr, ...) @scanf(ptr @1, ptr %6)
  %8 = load i32, ptr %i, align 4
  %9 = add i32 %8, 1
  store i32 %9, ptr %i, align 4
  %10 = load i32, ptr %i, align 4
  %11 = icmp ult i32 %10, 10
  br i1 %11, label %loop, label %after

after:                                            ; preds = %loop, %entry
  %i1 = alloca i32, align 4
  store i32 0, ptr %i1, align 4
  %12 = load i32, ptr %i1, align 4
  %13 = icmp ult i32 %12, 10
  br i1 %13, label %loop2, label %after3

loop2:                                            ; preds = %after5, %after
  %14 = load i32, ptr %i1, align 4
  %15 = add i32 %14, 1
  %j = alloca i32, align 4
  store i32 %15, ptr %j, align 4
  %16 = load i32, ptr %j, align 4
  %17 = icmp ult i32 %16, 10
  br i1 %17, label %loop4, label %after5

after3:                                           ; preds = %after5, %after
  %18 = call i32 (ptr, ...) @printf(ptr @2)
  %i6 = alloca i32, align 4
  store i32 0, ptr %i6, align 4
  %19 = load i32, ptr %i6, align 4
  %20 = icmp ult i32 %19, 10
  br i1 %20, label %loop7, label %after8

loop4:                                            ; preds = %after_if, %loop2
  %21 = load i32, ptr %i1, align 4
  %22 = getelementptr inbounds [10 x i32], ptr %array, i32 0, i32 %21
  %23 = load i32, ptr %22, align 4
  %24 = load i32, ptr %j, align 4
  %25 = getelementptr inbounds [10 x i32], ptr %array, i32 0, i32 %24
  %26 = load i32, ptr %25, align 4
  %27 = icmp ult i32 %23, %26
  br i1 %27, label %true_cond, label %false_cond

after5:                                           ; preds = %after_if, %loop2
  %28 = load i32, ptr %i1, align 4
  %29 = add i32 %28, 1
  store i32 %29, ptr %i1, align 4
  %30 = load i32, ptr %i1, align 4
  %31 = icmp ult i32 %30, 10
  br i1 %31, label %loop2, label %after3

true_cond:                                        ; preds = %loop4
  %32 = load i32, ptr %i1, align 4
  %33 = getelementptr inbounds [10 x i32], ptr %array, i32 0, i32 %32
  %34 = load i32, ptr %j, align 4
  %35 = getelementptr inbounds [10 x i32], ptr %array, i32 0, i32 %34
  call void @swap(ptr %33, ptr %35)
  br label %after_if

false_cond:                                       ; preds = %loop4
  br label %after_if

after_if:                                         ; preds = %true_cond, %false_cond
  %36 = load i32, ptr %j, align 4
  %37 = add i32 %36, 1
  store i32 %37, ptr %j, align 4
  %38 = load i32, ptr %j, align 4
  %39 = icmp ult i32 %38, 10
  br i1 %39, label %loop4, label %after5

loop7:                                            ; preds = %loop7, %after3
  %40 = load i32, ptr %i6, align 4
  %41 = getelementptr inbounds [10 x i32], ptr %array, i32 0, i32 %40
  %42 = load i32, ptr %41, align 4
  %43 = call i32 (ptr, ...) @printf(ptr @3, i32 %42)
  %44 = load i32, ptr %i6, align 4
  %45 = add i32 %44, 1
  store i32 %45, ptr %i6, align 4
  %46 = load i32, ptr %i6, align 4
  %47 = icmp ult i32 %46, 10
  br i1 %47, label %loop7, label %after8

after8:                                           ; preds = %loop7, %after3
  %48 = call i32 (ptr, ...) @printf(ptr @4)
  store i32 0, ptr %0, align 4
  br label %exit
}

; ModuleID = 'Id'
source_filename = "Id"

@0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@1 = private unnamed_addr constant [14 x i8] c"before sort: \00", align 1
@2 = private unnamed_addr constant [5 x i8] c"%d, \00", align 1
@3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@4 = private unnamed_addr constant [13 x i8] c"after sort: \00", align 1
@5 = private unnamed_addr constant [5 x i8] c"%d, \00", align 1
@6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

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
  %array = alloca [10 x i32], align 4
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  %0 = load i32, ptr %i, align 4
  %1 = icmp ult i32 %0, 10
  br i1 %1, label %loop, label %after

loop:                                             ; preds = %loop, %entry
  %2 = load i32, ptr %i, align 4
  %3 = getelementptr inbounds [10 x i32], ptr %array, i32 0, i32 %2
  %4 = call i32 (ptr, ...) @scanf(ptr @0, ptr %3)
  %5 = load i32, ptr %i, align 4
  %6 = add i32 %5, 1
  store i32 %6, ptr %i, align 4
  %7 = load i32, ptr %i, align 4
  %8 = icmp ult i32 %7, 10
  br i1 %8, label %loop, label %after

after:                                            ; preds = %loop, %entry
  %9 = call i32 (ptr, ...) @printf(ptr @1)
  %i1 = alloca i32, align 4
  store i32 0, ptr %i1, align 4
  %10 = load i32, ptr %i1, align 4
  %11 = icmp ult i32 %10, 10
  br i1 %11, label %loop2, label %after3

loop2:                                            ; preds = %loop2, %after
  %12 = load i32, ptr %i1, align 4
  %13 = getelementptr inbounds [10 x i32], ptr %array, i32 0, i32 %12
  %14 = load i32, ptr %13, align 4
  %15 = call i32 (ptr, ...) @printf(ptr @2, i32 %14)
  %16 = load i32, ptr %i1, align 4
  %17 = add i32 %16, 1
  store i32 %17, ptr %i1, align 4
  %18 = load i32, ptr %i1, align 4
  %19 = icmp ult i32 %18, 10
  br i1 %19, label %loop2, label %after3

after3:                                           ; preds = %loop2, %after
  %20 = call i32 (ptr, ...) @printf(ptr @3)
  %i4 = alloca i32, align 4
  store i32 0, ptr %i4, align 4
  %21 = load i32, ptr %i4, align 4
  %22 = icmp ult i32 %21, 10
  br i1 %22, label %loop5, label %after6

loop5:                                            ; preds = %after8, %after3
  %23 = load i32, ptr %i4, align 4
  %24 = add i32 %23, 1
  %j = alloca i32, align 4
  store i32 %24, ptr %j, align 4
  %25 = load i32, ptr %j, align 4
  %26 = icmp ult i32 %25, 10
  br i1 %26, label %loop7, label %after8

after6:                                           ; preds = %after8, %after3
  %27 = call i32 (ptr, ...) @printf(ptr @4)
  %i10 = alloca i32, align 4
  store i32 0, ptr %i10, align 4
  %28 = load i32, ptr %i10, align 4
  %29 = icmp ult i32 %28, 10
  br i1 %29, label %loop11, label %after12

loop7:                                            ; preds = %after9, %loop5
  %30 = load i32, ptr %i4, align 4
  %31 = getelementptr inbounds [10 x i32], ptr %array, i32 0, i32 %30
  %32 = load i32, ptr %31, align 4
  %33 = load i32, ptr %j, align 4
  %34 = getelementptr inbounds [10 x i32], ptr %array, i32 0, i32 %33
  %35 = load i32, ptr %34, align 4
  %36 = icmp ult i32 %32, %35
  br i1 %36, label %true_cond, label %false_cond

after8:                                           ; preds = %after9, %loop5
  %37 = load i32, ptr %i4, align 4
  %38 = add i32 %37, 1
  store i32 %38, ptr %i4, align 4
  %39 = load i32, ptr %i4, align 4
  %40 = icmp ult i32 %39, 10
  br i1 %40, label %loop5, label %after6

true_cond:                                        ; preds = %loop7
  %41 = load i32, ptr %i4, align 4
  %42 = getelementptr inbounds [10 x i32], ptr %array, i32 0, i32 %41
  %43 = load i32, ptr %j, align 4
  %44 = getelementptr inbounds [10 x i32], ptr %array, i32 0, i32 %43
  call void @swap(ptr %42, ptr %44)
  br label %after9

false_cond:                                       ; preds = %loop7
  br label %after9

after9:                                           ; preds = %true_cond, %false_cond
  %45 = load i32, ptr %j, align 4
  %46 = add i32 %45, 1
  store i32 %46, ptr %j, align 4
  %47 = load i32, ptr %j, align 4
  %48 = icmp ult i32 %47, 10
  br i1 %48, label %loop7, label %after8

loop11:                                           ; preds = %loop11, %after6
  %49 = load i32, ptr %i10, align 4
  %50 = getelementptr inbounds [10 x i32], ptr %array, i32 0, i32 %49
  %51 = load i32, ptr %50, align 4
  %52 = call i32 (ptr, ...) @printf(ptr @5, i32 %51)
  %53 = load i32, ptr %i10, align 4
  %54 = add i32 %53, 1
  store i32 %54, ptr %i10, align 4
  %55 = load i32, ptr %i10, align 4
  %56 = icmp ult i32 %55, 10
  br i1 %56, label %loop11, label %after12

after12:                                          ; preds = %loop11, %after6
  %57 = call i32 (ptr, ...) @printf(ptr @6)
  ret i32 0
}

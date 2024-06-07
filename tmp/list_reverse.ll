; ModuleID = 'Id'
source_filename = "Id"

@0 = private unnamed_addr constant [22 x i8] c"{next: %d, val: %d}, \00", align 1
@1 = private unnamed_addr constant [22 x i8] c"{next: %d, val: %d}, \00", align 1
@2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@3 = private unnamed_addr constant [22 x i8] c"{next: %d, val: %d}, \00", align 1
@4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(ptr, ...)

declare i32 @scanf(ptr, ...)

define i32 @main() {
entry:
  %0 = alloca i32, align 4
  %head = alloca i32, align 4
  store i32 0, ptr %head, align 4
  %memory = alloca [10 x { i32, i32 }], align 8
  %1 = load i32, ptr %head, align 4
  %i = alloca i32, align 4
  store i32 %1, ptr %i, align 4
  %2 = load i32, ptr %i, align 4
  %3 = icmp ult i32 %2, 9
  br i1 %3, label %loop, label %after

exit:                                             ; preds = %after6
  %4 = load i32, ptr %0, align 4
  ret i32 %4

loop:                                             ; preds = %loop, %entry
  %5 = load i32, ptr %i, align 4
  %6 = getelementptr inbounds [10 x { i32, i32 }], ptr %memory, i32 0, i32 %5
  %7 = getelementptr inbounds { i32, i32 }, ptr %6, i32 0, i32 0
  %8 = load i32, ptr %i, align 4
  %9 = add i32 %8, 1
  store i32 %9, ptr %7, align 4
  %10 = load i32, ptr %i, align 4
  %11 = getelementptr inbounds [10 x { i32, i32 }], ptr %memory, i32 0, i32 %10
  %12 = getelementptr inbounds { i32, i32 }, ptr %11, i32 0, i32 1
  %13 = load i32, ptr %i, align 4
  %14 = add i32 %13, 1
  %15 = mul i32 %14, 10
  store i32 %15, ptr %12, align 4
  %16 = load i32, ptr %i, align 4
  %17 = getelementptr inbounds [10 x { i32, i32 }], ptr %memory, i32 0, i32 %16
  %18 = getelementptr inbounds { i32, i32 }, ptr %17, i32 0, i32 0
  %19 = load i32, ptr %18, align 4
  %20 = load i32, ptr %i, align 4
  %21 = getelementptr inbounds [10 x { i32, i32 }], ptr %memory, i32 0, i32 %20
  %22 = getelementptr inbounds { i32, i32 }, ptr %21, i32 0, i32 1
  %23 = load i32, ptr %22, align 4
  %24 = call i32 (ptr, ...) @printf(ptr @0, i32 %19, i32 %23)
  %25 = load i32, ptr %i, align 4
  %26 = add i32 %25, 1
  store i32 %26, ptr %i, align 4
  %27 = load i32, ptr %i, align 4
  %28 = icmp ult i32 %27, 9
  br i1 %28, label %loop, label %after

after:                                            ; preds = %loop, %entry
  %29 = getelementptr inbounds [10 x { i32, i32 }], ptr %memory, i32 0, i32 9
  %30 = getelementptr inbounds { i32, i32 }, ptr %29, i32 0, i32 0
  store i32 -1, ptr %30, align 4
  %31 = getelementptr inbounds [10 x { i32, i32 }], ptr %memory, i32 0, i32 9
  %32 = getelementptr inbounds { i32, i32 }, ptr %31, i32 0, i32 1
  store i32 110, ptr %32, align 4
  %33 = getelementptr inbounds [10 x { i32, i32 }], ptr %memory, i32 0, i32 9
  %34 = getelementptr inbounds { i32, i32 }, ptr %33, i32 0, i32 0
  %35 = load i32, ptr %34, align 4
  %36 = getelementptr inbounds [10 x { i32, i32 }], ptr %memory, i32 0, i32 9
  %37 = getelementptr inbounds { i32, i32 }, ptr %36, i32 0, i32 1
  %38 = load i32, ptr %37, align 4
  %39 = call i32 (ptr, ...) @printf(ptr @1, i32 %35, i32 %38)
  %40 = call i32 (ptr, ...) @printf(ptr @2)
  %new_head = alloca i32, align 4
  store i32 9, ptr %new_head, align 4
  %new_memory = alloca [10 x { i32, i32 }], align 8
  %41 = load i32, ptr %new_head, align 4
  %42 = getelementptr inbounds [10 x { i32, i32 }], ptr %new_memory, i32 0, i32 %41
  %43 = getelementptr inbounds { i32, i32 }, ptr %42, i32 0, i32 0
  store i32 -1, ptr %43, align 4
  %i1 = alloca i32, align 4
  store i32 0, ptr %i1, align 4
  %44 = load i32, ptr %i1, align 4
  %45 = icmp ult i32 %44, 9
  br i1 %45, label %loop2, label %after3

loop2:                                            ; preds = %loop2, %after
  %46 = load i32, ptr %new_head, align 4
  %47 = getelementptr inbounds [10 x { i32, i32 }], ptr %new_memory, i32 0, i32 %46
  %48 = getelementptr inbounds { i32, i32 }, ptr %47, i32 0, i32 1
  %49 = load i32, ptr %head, align 4
  %50 = getelementptr inbounds [10 x { i32, i32 }], ptr %memory, i32 0, i32 %49
  %51 = getelementptr inbounds { i32, i32 }, ptr %50, i32 0, i32 1
  %52 = load i32, ptr %51, align 4
  store i32 %52, ptr %48, align 4
  %53 = load i32, ptr %head, align 4
  %54 = getelementptr inbounds [10 x { i32, i32 }], ptr %memory, i32 0, i32 %53
  %55 = getelementptr inbounds { i32, i32 }, ptr %54, i32 0, i32 0
  %56 = load i32, ptr %55, align 4
  store i32 %56, ptr %head, align 4
  %57 = load i32, ptr %new_head, align 4
  %58 = sub i32 %57, 1
  %new_node = alloca i32, align 4
  store i32 %58, ptr %new_node, align 4
  %59 = load i32, ptr %new_node, align 4
  %60 = getelementptr inbounds [10 x { i32, i32 }], ptr %new_memory, i32 0, i32 %59
  %61 = getelementptr inbounds { i32, i32 }, ptr %60, i32 0, i32 0
  %62 = load i32, ptr %new_head, align 4
  store i32 %62, ptr %61, align 4
  %63 = load i32, ptr %new_node, align 4
  store i32 %63, ptr %new_head, align 4
  %64 = load i32, ptr %i1, align 4
  %65 = add i32 %64, 1
  store i32 %65, ptr %i1, align 4
  %66 = load i32, ptr %i1, align 4
  %67 = icmp ult i32 %66, 9
  br i1 %67, label %loop2, label %after3

after3:                                           ; preds = %loop2, %after
  %68 = load [10 x { i32, i32 }], ptr %new_memory, align 4
  store [10 x { i32, i32 }] %68, ptr %memory, align 4
  %69 = load i32, ptr %new_head, align 4
  store i32 %69, ptr %head, align 4
  %i4 = alloca i32, align 4
  store i32 0, ptr %i4, align 4
  %70 = load i32, ptr %i4, align 4
  %71 = icmp ult i32 %70, 10
  br i1 %71, label %loop5, label %after6

loop5:                                            ; preds = %loop5, %after3
  %72 = load i32, ptr %i4, align 4
  %73 = getelementptr inbounds [10 x { i32, i32 }], ptr %memory, i32 0, i32 %72
  %74 = getelementptr inbounds { i32, i32 }, ptr %73, i32 0, i32 0
  %75 = load i32, ptr %74, align 4
  %76 = load i32, ptr %i4, align 4
  %77 = getelementptr inbounds [10 x { i32, i32 }], ptr %memory, i32 0, i32 %76
  %78 = getelementptr inbounds { i32, i32 }, ptr %77, i32 0, i32 1
  %79 = load i32, ptr %78, align 4
  %80 = call i32 (ptr, ...) @printf(ptr @3, i32 %75, i32 %79)
  %81 = load i32, ptr %i4, align 4
  %82 = add i32 %81, 1
  store i32 %82, ptr %i4, align 4
  %83 = load i32, ptr %i4, align 4
  %84 = icmp ult i32 %83, 10
  br i1 %84, label %loop5, label %after6

after6:                                           ; preds = %loop5, %after3
  %85 = call i32 (ptr, ...) @printf(ptr @4)
  store i32 0, ptr %0, align 4
  br label %exit
}

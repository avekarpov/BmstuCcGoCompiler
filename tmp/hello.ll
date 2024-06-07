; ModuleID = 'Id'
source_filename = "Id"

@0 = private unnamed_addr constant [7 x i8] c"hello\0A\00", align 1

declare i32 @printf(ptr, ...)

declare i32 @scanf(ptr, ...)

define i32 @main() {
entry:
  %0 = alloca i32, align 4
  %1 = call i32 (ptr, ...) @printf(ptr @0)

exit:                                             ; No predecessors!
  %2 = load i32, ptr %0, align 4
  ret i32 %2
}

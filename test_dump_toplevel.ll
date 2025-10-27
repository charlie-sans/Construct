; ModuleID = 'construct'
source_filename = "construct"

@str = private unnamed_addr constant [14 x i8] c"Hello, World!\00", align 1

declare i32 @printf(ptr, ...)

declare i32 @puts(ptr)

declare ptr @malloc(i64)

declare void @free(ptr)

declare i64 @strlen(ptr)

declare ptr @strcpy(ptr, ptr)

declare ptr @strcat(ptr, ptr)

declare i32 @strcmp(ptr, ptr)

declare void @exit(i32)

declare ptr @getenv(ptr)

declare i32 @system(ptr)

declare void @construct_dump_int(i32)

declare void @construct_dump_float(double)

declare void @construct_dump_bool(i1)

declare void @construct_dump_string(ptr)

define i32 @main() {
entry:
  call void @construct_dump_int(i32 42)
  call void @construct_dump_string(ptr @str)
  call void @construct_dump_bool(i1 true)
  ret i32 1
}

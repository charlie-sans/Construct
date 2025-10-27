; ModuleID = 'construct'
source_filename = "construct"

@str = private unnamed_addr constant [6 x i8] c"Meow!\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"Meow!\00", align 1

declare i32 @printf(ptr, ...)

declare i32 @puts(ptr)

declare void @fdump(ptr, ptr)

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
  ret i32 0
}

declare i32 @print(ptr)

define i32 @meow() {
entry:
  %0 = call i32 @print(ptr @str.1)
  ret i32 %0
}

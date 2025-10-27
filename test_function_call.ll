; ModuleID = 'construct'
source_filename = "construct"

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
  %0 = call i32 @add(i32 3, i32 7)
  call void @construct_dump_int(i32 %0)
  ret i32 %0
}

define i32 @add(i32 %0, i32 %1) {
entry:
  %addtmp = add i32 %0, %1
  ret i32 %addtmp
}

; ModuleID = 'construct'
source_filename = "construct"

%0 = type { i32, i32 }

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
  %record_ptr = call ptr @malloc(i64 8)
  %field_ptr_x = getelementptr %0, ptr %record_ptr, i32 0, i32 0
  store i32 10, ptr %field_ptr_x, align 4
  %field_ptr_y = getelementptr %0, ptr %record_ptr, i32 0, i32 1
  store i32 20, ptr %field_ptr_y, align 4
  call void @construct_dump_int(i32 42)
  ret i32 0
}

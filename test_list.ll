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
  %list_ptr = call ptr @malloc(i64 28)
  store i64 5, ptr %list_ptr, align 4
  %list_data_start = getelementptr i64, ptr %list_ptr, i64 1
  %elem_ptr = getelementptr i32, ptr %list_data_start, i64 0
  store i32 1, ptr %elem_ptr, align 4
  %elem_ptr1 = getelementptr i32, ptr %list_data_start, i64 1
  store i32 2, ptr %elem_ptr1, align 4
  %elem_ptr2 = getelementptr i32, ptr %list_data_start, i64 2
  store i32 3, ptr %elem_ptr2, align 4
  %elem_ptr3 = getelementptr i32, ptr %list_data_start, i64 3
  store i32 4, ptr %elem_ptr3, align 4
  %elem_ptr4 = getelementptr i32, ptr %list_data_start, i64 4
  store i32 5, ptr %elem_ptr4, align 4
  call void @construct_dump_int(i32 42)
  ret i32 0
}

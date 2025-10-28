; ModuleID = 'construct'
source_filename = "construct"

declare i32 @printf(ptr, ...)

declare i32 @puts(ptr)

declare ptr @readline(ptr)

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

declare void @construct_dump_int_line(i32)

declare void @construct_dump_float_line(double)

declare void @construct_dump_bool_line(i1)

declare void @construct_dump_string_line(ptr)

declare void @construct_nl()

declare i32 @construct_read_int()

declare double @construct_read_float()

declare i1 @construct_read_bool()

declare ptr @construct_read_string()

declare ptr @input(ptr)

declare ptr @construct_read_line()

declare ptr @construct_read_line_prompt(ptr)

declare ptr @construct_input(ptr)

declare ptr @construct_string_upper(ptr)

declare ptr @construct_string_lower(ptr)

declare ptr @construct_string_trim(ptr)

declare ptr @construct_string_concat(ptr, ptr)

declare i1 @construct_string_equals(ptr, ptr)

declare i1 @construct_string_starts_with(ptr, ptr)

declare i1 @construct_string_ends_with(ptr, ptr)

declare i32 @construct_string_length(ptr)

declare i32 @construct_abs_int(i32)

declare i32 @construct_max_int(i32, i32)

declare i32 @construct_min_int(i32, i32)

declare i32 @construct_clamp_int(i32, i32, i32)

declare double @construct_abs_float(double)

declare double @construct_max_float(double, double)

declare double @construct_min_float(double, double)

declare double @construct_clamp_float(double, double, double)

declare i32 @construct_round(double)

declare i32 @construct_floor(double)

declare i32 @construct_ceil(double)

declare ptr @construct_int_to_string(i32)

declare ptr @construct_float_to_string(double)

declare ptr @construct_bool_to_string(i1)

declare i32 @construct_string_to_int(ptr)

declare double @construct_string_to_float(ptr)

declare i1 @construct_string_to_bool(ptr)

declare void @construct_sleep(i32)

declare void @construct_sleep_ms(i32)

declare void @construct_clear_screen()

declare void @construct_construct_exit(i32)

declare void @construct_print_error(ptr)

declare void @construct_debug(ptr)

declare void @construct_show_int(i32)

declare void @construct_show_float(double)

declare void @construct_show_bool(i1)

declare void @construct_show_string(ptr)

declare void @construct_showln_int(i32)

declare void @construct_showln_float(double)

declare void @construct_showln_bool(i1)

declare void @construct_showln_string(ptr)

declare void @construct_showln()

define i32 @main() {
entry:
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  br i1 true, label %while.body, label %while.exit

while.body:                                       ; preds = %while.cond
  %0 = call i32 @test()
  %1 = call i32 @test2()
  br label %while.cond

while.exit:                                       ; preds = %while.cond
  ret i32 0
}

declare i32 @test()

declare i32 @test2()

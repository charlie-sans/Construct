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
  %0 = call i32 @add(i32 10, i32 5)
  %1 = call i32 @subtract(i32 20, i32 8)
  %2 = call i32 @multiply(i32 6, i32 7)
  %3 = call i32 @absolute(i32 -15)
  %4 = call i32 @square(i32 5)
  %5 = call i32 @double(i32 7)
  %6 = call i1 @is_positive(i32 42)
  %7 = call i1 @is_positive(i32 -10)
  %8 = call i1 @is_even(i32 10)
  %9 = call i1 @is_even(i32 7)
  %10 = call i1 @is_greater(i32 15, i32 10)
  %11 = call i1 @is_greater(i32 5, i32 20)
  %12 = call i1 @is_positive_and_even(i32 8)
  call void @construct_dump_int(i32 %0)
  call void @construct_dump_int(i32 %1)
  call void @construct_dump_int(i32 %2)
  call void @construct_dump_int(i32 %3)
  call void @construct_dump_int(i32 %4)
  call void @construct_dump_int(i32 %5)
  call void @construct_dump_bool(i1 %6)
  call void @construct_dump_bool(i1 %7)
  call void @construct_dump_bool(i1 %8)
  call void @construct_dump_bool(i1 %9)
  call void @construct_dump_bool(i1 %10)
  call void @construct_dump_bool(i1 %11)
  call void @construct_dump_bool(i1 %12)
  %13 = zext i1 %12 to i32
  ret i32 %13
}

define i32 @add(i32 %0, i32 %1) {
entry:
  %addtmp = add i32 %0, %1
  ret i32 %addtmp
}

define i32 @subtract(i32 %0, i32 %1) {
entry:
  %subtmp = sub i32 %0, %1
  ret i32 %subtmp
}

define i32 @multiply(i32 %0, i32 %1) {
entry:
  %multmp = mul i32 %0, %1
  ret i32 %multmp
}

define i32 @absolute(i32 %0) {
entry:
  %lttmp = icmp slt i32 %0, 0
  br i1 %lttmp, label %then, label %else

then:                                             ; preds = %entry
  %subtmp = sub i32 0, %0
  br label %merge

else:                                             ; preds = %entry
  br label %merge

merge:                                            ; preds = %else, %then
  %iftmp = phi i32 [ %subtmp, %then ], [ %0, %else ]
  ret i32 %iftmp
}

define i1 @is_positive(i32 %0) {
entry:
  %gttmp = icmp sgt i32 %0, 0
  ret i1 %gttmp
}

define i1 @is_even(i32 %0) {
entry:
  %remtmp = srem i32 %0, 2
  %eqtmp = icmp eq i32 %remtmp, 0
  ret i1 %eqtmp
}

define i1 @is_greater(i32 %0, i32 %1) {
entry:
  %gttmp = icmp sgt i32 %0, %1
  ret i1 %gttmp
}

define i32 @square(i32 %0) {
entry:
  %1 = call i32 @multiply(i32 %0, i32 %0)
  ret i32 %1
}

define i32 @double(i32 %0) {
entry:
  %1 = call i32 @add(i32 %0, i32 %0)
  ret i32 %1
}

define i1 @is_positive_and_even(i32 %0) {
entry:
  %1 = call i1 @is_even(i32 %0)
  ret i1 %1
}

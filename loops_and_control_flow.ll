; ModuleID = 'construct'
source_filename = "construct"

@str = private unnamed_addr constant [6 x i8] c"adult\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"minor\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"one\00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"two\00", align 1
@str.4 = private unnamed_addr constant [6 x i8] c"other\00", align 1

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
  br i1 true, label %then, label %else

then:                                             ; preds = %entry
  br label %merge

else:                                             ; preds = %entry
  br label %merge

merge:                                            ; preds = %else, %then
  %iftmp = phi ptr [ @str, %then ], [ @str.1, %else ]
  %0 = call i32 @add(i32 10, i32 5)
  %1 = call i32 @multiply(i32 4, i32 3)
  %2 = call i32 @square(i32 7)
  %3 = call i1 @is_positive(i32 42)
  %4 = call i1 @is_positive(i32 -5)
  br label %case

match_merge:                                      ; preds = %case2, %case1, %case
  %matchtmp = phi ptr [ @str.2, %case ], [ @str.3, %case1 ], [ @str.4, %case2 ]
  call void @construct_dump_string(ptr %iftmp)
  call void @construct_dump_int(i32 %0)
  call void @construct_dump_int(i32 %1)
  call void @construct_dump_int(i32 %2)
  call void @construct_dump_bool(i1 %3)
  call void @construct_dump_bool(i1 %4)
  call void @construct_dump_string(ptr %matchtmp)
  ret i32 0

case:                                             ; preds = %merge
  br label %match_merge

case1:                                            ; No predecessors!
  br label %match_merge

case2:                                            ; No predecessors!
  br label %match_merge
}

define i32 @add(i32 %0, i32 %1) {
entry:
  %addtmp = add i32 %0, %1
  ret i32 %addtmp
}

define i32 @multiply(i32 %0, i32 %1) {
entry:
  %multmp = mul i32 %0, %1
  ret i32 %multmp
}

define i32 @square(i32 %0) {
entry:
  %1 = call i32 @multiply(i32 %0, i32 %0)
  ret i32 %1
}

define i1 @is_positive(i32 %0) {
entry:
  %gttmp = icmp sgt i32 %0, 0
  ret i1 %gttmp
}

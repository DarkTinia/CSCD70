; ModuleID = './test/2-Liveness.ll'
source_filename = "./test/2-Liveness.ll"

define i32 @sum(i32 noundef %0, i32 noundef %1) {
  br label %3

3:                                                ; preds = %7, %2
  %.01 = phi i32 [ 1, %2 ], [ %6, %7 ]
  %.0 = phi i32 [ %0, %2 ], [ %8, %7 ]
  %4 = icmp slt i32 %.0, %1
  br i1 %4, label %5, label %9

5:                                                ; preds = %3
  %6 = add nsw i32 %.01, %.0
  br label %7

7:                                                ; preds = %5
  %8 = add nsw i32 %.0, 1
  br label %3

9:                                                ; preds = %3
  ret i32 %.01
}

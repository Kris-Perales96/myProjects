module passSignal(a,w);
input [7:0] a;
output [7:0] w;

and and0(w[0], a[0], 1'b1);
and and1(w[1], a[1], 1'b1);
and and2(w[2], a[2], 1'b1);
and and3(w[3], a[3], 1'b1);
and and4(w[4], a[4], 1'b1);
and and5(w[5], a[5], 1'b1);
and and6(w[6], a[6], 1'b1);
and and7(w[7], a[7], 1'b1);

endmodule

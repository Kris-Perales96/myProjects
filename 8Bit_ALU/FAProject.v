module FAProject(a,b,cin,cout,sum);
	input  a, b, cin;
	output sum, cout;
	wire w,w1,w2;
	
	xor xor01(w,a,b);
	xor xor02(sum, w, cin);
	
	and a1(w1,w,cin);	
	and a2(w2, a, b);		
	or o1(cout, w1,w2);		
	
endmodule

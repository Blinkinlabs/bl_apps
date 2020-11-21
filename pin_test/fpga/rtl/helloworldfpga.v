module helloworldfpga(
    input BUTTON,
    output wire LED,

    output wire OUT_0,
    output wire OUT_1,
    output wire OUT_2,
    output wire OUT_3,
    output wire OUT_4,
    output wire OUT_5,
    output wire OUT_6,
    output wire OUT_7,
    output wire OUT_8,
    output wire OUT_9,

    output wire OUT_10,
    output wire OUT_11,
    output wire OUT_12,
    output wire OUT_13,
    output wire OUT_14,
    output wire OUT_15,
    output wire OUT_16,
    output wire OUT_17,
    output wire OUT_18,
    output wire OUT_19,

    output wire OUT_20,
    output wire OUT_21,
    output wire OUT_22,
    output wire OUT_23,
    output wire OUT_24,
    output wire OUT_25,
);
    wire clk;

    qlal4s3b_cell_macro u_qlal4s3b_cell_macro (
        .Sys_Clk0 (clk),
    );

    reg	[23:0]	cnt;
    reg	[23:0]	stopcnt;
    reg		led;
    initial cnt <= 0;
    initial stopcnt <= 4000000;		// Change to 2000000
    initial led <= 0;

    always @(posedge clk) begin
	if (cnt == stopcnt) begin
		cnt <= 0;
		led <= ~led;
	end else begin
        cnt <= cnt + 1;
	end
    end
    assign LED = led & BUTTON;	// Change to redled = led;

    reg [25:0] out_shift;
    initial out_shift <= 26'b1;

    always @(posedge clk) begin
        out_shift <= (out_shift << 1);
        if(out_shift == 0) begin
            out_shift <= 26'b1;
        end
    end
    

    assign {OUT_0, OUT_1, OUT_2, OUT_3, OUT_4, OUT_5, OUT_6, OUT_7, OUT_8, OUT_9, 
            OUT_10, OUT_11, OUT_12, OUT_13, OUT_14, OUT_15, OUT_16, OUT_17, OUT_18, OUT_19, 
            OUT_20, OUT_21, OUT_22, OUT_23, OUT_24, OUT_25} = out_shift[25:0];
endmodule

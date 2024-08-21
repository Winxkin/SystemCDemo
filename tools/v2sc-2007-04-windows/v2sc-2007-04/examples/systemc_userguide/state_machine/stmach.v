

module stmach(clk, key, play, recrd, erase, save,address);

`include "def.v"

input clk;
input [0:3] key;
output play, recrd, erase, save, address;

reg [0:3] next_state;
reg [0:3] current_state;
reg play, recrd, erase, save, address;

always @(posedge clk)
	current_state = next_state;
	
always @(key or current_state)
begin
	play = 1'b0;
	recrd = 1'b0;
	erase = 1'b0;
	save = 1'b0;
	address = 1'b0;
	case (current_state)
		main_st : 
		begin
			if (key == one)
				next_state = review_st;
			else if (key == two)
				next_state = send_st;
			else
				next_state = main_st;
		end
		review_st:
		begin
			if (key == one)
				next_state = repeat_st;
			else if (key == two)
				next_state = save_st;
			else if (key == three)
				next_state = erase_st;
			else if (key == pound)
				next_state = main_st;
			else
			next_state = review_st;
		end
		repeat_st: 
		begin
			play = 1'b1;
			next_state = review_st;
		end
		save_st:
		begin
			save = 1'b1;
			next_state = review_st;
		end
		erase_st:
		begin
			erase = 1'b1;
			next_state = review_st;
		end
		send_st:
		begin
			next_state = address_st;
		end
		address_st:
		begin
			address = 1'b1;
			if (key == pound)
				next_state = record_st;
			else
				next_state = address_st;
		end
		record_st: 
		begin
			if (key == five)
				next_state = begin_rec_st;
			else
				next_state = record_st;
		end
		begin_rec_st: 
		begin
			recrd = 1'b1;
			next_state = message_st;
		end
		message_st: 
		begin
			recrd = 1'b1;
			if (key == pound)
				next_state = send_st;
			else
			next_state = message_st;
		end
	endcase
end

endmodule
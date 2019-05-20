sig io.
#lib io.

regcl string_to_term, readterm, read, lookahead, input, input_line.

extern type open_in         openIn      string -> in_stream -> o. 
extern type open_out        openOut     string -> out_stream -> o.
extern type open_append     openApp     string -> out_stream -> o.
extern type close_in        closeIn     in_stream -> o.
extern type close_out       closeOut    out_stream -> o.
extern type open_string     openStr     string -> in_stream -> o.
extern type input           input       in_stream -> int -> string -> o.
extern type output          output      out_stream -> string -> o.
extern type input_line      inputLine   in_stream -> string -> o.
extern type lookahead       lookahead   in_stream -> string -> o.
extern type eof             eof	        in_stream -> o.
extern type flush           flush       out_stream -> o.
extern type print           print       string -> o.
extern type read            read        A -> o.
extern type printterm       printTerm   out_stream -> A -> o.
extern type term_to_string  termToStr   A -> string -> o.
extern type string_to_term  strToTerm   string -> A -> o.
extern type readterm        readTerm    in_stream -> A -> o.

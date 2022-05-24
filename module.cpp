#include "s7.h"

#include <emscripten/bind.h>
#include <emscripten/emscripten.h>

#include <cstdio>
#include <cstring>

#include <string>
#include <algorithm>

extern "C" void run(const std::string& code) noexcept {;
	auto* s7 = s7_init();
	// do (more) stuff pwease
	s7_define_function(s7, "draw-pixel", [](s7_scheme* sc, s7_pointer args) -> s7_pointer {
		auto x = s7_car(args);
		auto y = s7_cadr(args);
		auto color = s7_caddr(args);
		auto nchannels = s7_vector_length(color);
		if (!s7_is_integer(x)) {
			EM_ASM("console.log(\"didn't print stuff\")");
			return s7_wrong_type_arg_error(sc, "draw-pixel", 1, x, "must be an integer");
		}
		if (!s7_is_integer(y)) {
			EM_ASM("console.log(\"didn't print stuff\")");
			return s7_wrong_type_arg_error(sc, "draw-pixel", 2, y, "must be an integer");
		}
		if (!s7_is_byte_vector(color) || !(nchannels == 3 || nchannels == 4)) {
			EM_ASM("console.log(\"didn't print stuff\")");
			return s7_wrong_type_arg_error(sc, "draw-pixel", 3, color, "must be a 3 or 4 elements byte vector");
		}
		if (nchannels == 3) {
			EM_ASM({
					ctx.fillStyle = "rgb(" + $2 + "," + $3 + "," + $4 + "})";
					ctx.fillRect($0, $1, 1, 1);
				},
				s7_integer(x),
				s7_integer(y),
				s7_byte_vector_ref(color, 0),
				s7_byte_vector_ref(color, 1),
				s7_byte_vector_ref(color, 2)
			);
			EM_ASM("console.log(\"printed rgb\")");
		}
		else {
			EM_ASM({
				ctx.fillStyle = "rgba(" + $2 + "," + $3 + "," + $4 + "," + ($5 / 255) + ")";
				ctx.fillRect($0, $1, 1, 1);
				}, 
				s7_integer(x),
				s7_integer(y),
				s7_byte_vector_ref(color, 0),
				s7_byte_vector_ref(color, 1),
				s7_byte_vector_ref(color, 2),
				s7_byte_vector_ref(color, 3)
			);
			EM_ASM("console.log(\"printed rgba\")");
		}
		EM_ASM("console.log(\"printed stuff\")");
		return s7_nil(sc);
	}, 3, 0, false, "(draw-pixel <int> <int> <color>)");
	s7_define_constant(s7, "screen-width", s7_make_integer(s7, 512));
	s7_define_constant(s7, "screen-height", s7_make_integer(s7, 512));
	
	//auto first = code.begin();
	//auto last = code.end();
	//while (first != last) {
	//	const auto end_of_line_it = std::find(first, last, '\n');
	//	s7_eval_c_string(s7, std::string{ first, end_of_line_it }.c_str());
	//	first = end_of_line_it;
	//}

	s7_eval(s7, s7_load_c_string(s7, code.c_str(), code.size()), s7_nil(s7));
	s7_free(s7);
}

EMSCRIPTEN_BINDINGS(draw_module) {
	emscripten::function("run", &run);
}
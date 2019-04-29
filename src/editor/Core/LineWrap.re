/**
 * LineWrap.re
 * 
 * Logic for wrapping lines in Onivim 2
 */

open Types;

type wrap = {
	/*
	 * The UTF-8 index of the start of a wrapped (virtual) line
	 */
	index: int,

	/*
	 * The length of the virtual line
	 */
	length: int,
};

type t = array(wrap);

let empty = [|{index: 0, length: 0}|];

let create: (string, int) => t = (s, width) => {
	let len = Zed_utf8.length(s);
	let idx = ref(0);
	let wraps = ref([]);

	/* 
		Very naive logic for line wrapping!

		Should take into account:
		- Multi-width characters (logic assumes every character is 1 width)
		- Different wrapping strategies (wrap _words_)
		- Matching indent of parent line
		- Showing indent character
	*/
	while (idx^ < len) {
		let i = idx^;
		let segment = min(width, len - i);
		let wrap = {
			index: i,
			length: i + segment,
		};
		wraps := [wrap, ...wraps^];
		idx := i + segment;
	};

	let ret = wraps^
	|> List.rev
	|> Array.of_list;

	switch (Array.length(ret)) {
	| 0 => empty
	| _ => ret
	};

};

let count: (t) => int = (v) => Array.length(v);

let toVirtualPosition: (Index.t, t) => Position.t = (character, _v) => {
	let c = Index.toInt0(character);

    Position.fromIndices0(0, c);
};

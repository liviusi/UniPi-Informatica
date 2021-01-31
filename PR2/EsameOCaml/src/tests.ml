let e0 = emptyEnv;;
(* EXPECTED: val e0 : (string * evT) list = [("", Unbound)] *)

eval (Empty("int")) e0;;
(* EXPECTED: - : evT = Set ("int", [Unbound]) *)

eval (Empty("string")) e0;;
(* EXPECTED: - : evT = Set ("string", [Unbound]) *)

eval (Empty("bool")) e0;;
(* EXPECTED: - : evT = Set ("bool", [Unbound]) *)

eval (Empty("set")) e0;;
(* EXPECTED: Exception: Failure "Empty: cannot declare empty set of given type.". *)

eval (Singleton(CstFalse, "bool")) e0;;
(* EXPECTED: - : evT = Set ("bool", [Bool false]) *)

eval (Singleton(CstFalse, "int")) e0;;
(* EXPECTED: Exception: Failure "Set: set contains illegal elements (there may be duplicates and/or elements of different types).". *)

eval (Singleton(EString("A robot may not injure a human being or, through inaction, allow a human being to come to harm."), "string")) e0;;
(* EXPECTED: - : evT = Set ("string", [String "A robot may not injure a human being or, through inaction, allow a human being to come to harm."]) *)

eval (Of("string", [])) e0;;
(* EXPECTED: - : evT = Set ("string", [Unbound]) *)

eval (Of("string", [EString("lorem")])) e0;;
(* EXPECTED: - : evT = Set ("string", [String "lorem"]) *)

eval (Of("string", [EString("lorem") ; EString("ipsum") ; EString("dolor") ])) e0;;
(* EXPECTED: - : evT = Set ("string", [String "dolor"; String "ipsum"; String "lorem"]) *)

eval (Of("int", [CstInt(2); CstInt(42); CstInt(3)] )) e0;;
(* EXPECTED: - : evT = Set ("int", [Int 3; Int 42; Int 2]) *)

eval (Of("set", [Of("int", [CstInt(2); CstInt(42); CstInt(3)]) ; Of("int", [CstInt(2); CstInt(3)])])) e0;;
(* EXPECTED: Exception: Failure "Set: invalid set type.". *)

eval (Of("bool", [CstInt(2); CstInt(42); CstInt(3)] )) e0;;
(* EXPECTED: Exception: Failure "Set: set contains illegal elements (there may be duplicates and/or elements of different types).". *)

eval (Of("int", [CstInt(42); CstInt(42); CstInt(3)] )) e0;;
(* EXPECTED: Exception: Failure "Set: set contains illegal elements (there may be duplicates and/or elements of different types).". *)


let s = Of("int", [CstInt(2); CstInt(42); CstInt(3)]) in
	let i = CstInt(5) in
		eval (Add(s, i)) e0;;
(* EXPECTED: - : evT = Set ("int", [Int 5; Int 3; Int 42; Int 2]) *)

let s = Of("int", [CstInt(2); CstInt(42); CstInt(3)]) in
	let i = CstInt(42) in
		eval (Add(s, i)) e0;;
(* EXPECTED: - : evT = Set ("int", [Int 3; Int 42; Int 2]) *)

let s = CstInt(42) in
	let i = CstInt(42) in
		eval (Add(s, i)) e0;;
(* EXPECTED: Exception: Failure "Add: cannot run on non set types.". *)

let s = Empty("int") in
	let i = CstInt(42) in
		eval (Add(s, i)) e0;;;;
(* EXPECTED: - : evT = Set ("int", [Int 42]) *)

let s = Empty("bool") in
	let i = CstInt(42) in
		eval (Add(s, i)) e0;;;;
(* EXPECTED: Exception: Failure "Add: element of type to be added does not match set's.". *)

let s = Of("int", [CstInt(2); CstInt(3)]) in
	let i = CstInt(2) in
		eval (Remove(s, i)) e0;;
(* EXPECTED: - : evT = Set ("int", [Int 3]) *)

let s = Of("int", [CstInt(2); CstInt(3)]) in
	let i = CstInt(42) in
		eval (Remove(s, i)) e0;;
(* EXPECTED: - : evT = Set ("int", [Int 3; Int 2]) *)

let s = Of("int", [CstInt(2); CstInt(3)]) in
	let i = CstTrue in
		eval (Remove(s, i)) e0;;
(* EXPECTED: - : evT = Set ("int", [Int 3; Int 2]) *)

let s = CstInt(42) in
	let i = CstInt(42) in
		eval (Remove(s, i)) e0;;
(* EXPECTED: Exception: Failure "Remove: cannot run on non set types.". *)

let s = Empty("int") in
	let i = CstInt(42) in
		eval (Remove(s, i)) e0;;
(* EXPECTED: - : evT = Set ("int", [Unbound]) *)

let s = Of("string", [EString("lorem") ; EString("ipsum") ; EString("dolor") ]) in
	let i = EString("ipsum") in
		eval (Remove(s, i)) e0;;
(* EXPECTED: - : evT = Set ("string", [String "dolor"; String "lorem"]) *)

let s = Empty("int") in
	eval (IsEmpty(s)) e0;;
(* EXPECTED: - : evT = Bool true *)

let s = Of("int", [CstInt(2); CstInt(42); CstInt(3)]) in
	eval (IsEmpty(s)) e0;;
(* EXPECTED: - : evT = Bool false *)

let s = CstInt(42) in
	eval (IsEmpty(s)) e0;;
(* EXPECTED: Exception: Failure "IsEmpty: cannot run on non set types.". *)

let s = Of("string", [EString("lorem") ; EString("ipsum") ; EString("dolor") ]) in
	let i = EString("ipsum") in
		eval (BelongsTo(s, i)) e0;;
(* EXPECTED: - : evT = Bool true *)

let s = Of("string", [EString("lorem") ; EString("ipsum") ; EString("dolor") ]) in
	let i = CstInt(2) in
		eval (BelongsTo(s, i)) e0;;
(* EXPECTED: - : evT = Bool false *)

let s = CstInt(42) in
	let i = CstInt(42) in
		eval (BelongsTo(s, i)) e0;;
(* EXPECTED: Exception: Failure "BelongsTo: cannot run on non set types.". *)

let s1 = Of("string", [EString("lorem") ; EString("ipsum")]) in
	let s2 = Of("string", [EString("lorem") ; EString("ipsum") ; EString("dolor") ]) in
		eval (IsSubset(s1, s2)) e0;;
(* EXPECTED: - : evT = Bool true *)

let s1 = Of("string", [EString("lorem") ; EString("ipsum")]) in
	let s2 = Of("string", [EString("lorem") ; EString("ipsum") ; EString("dolor") ]) in
		eval (IsSubset(s2, s1)) e0;;
(* EXPECTED: - : evT = Bool false *)

let s1 = Empty("bool") in
	let s2 = Of("string", [EString("lorem") ; EString("ipsum") ; EString("dolor") ]) in
		eval (IsSubset(s1, s2)) e0;;
(* EXPECTED: - : evT = Bool false *)

let s1 = Empty("bool") in
	let s2 = Of("bool", [CstTrue ; CstFalse]) in
		eval (IsSubset(s1, s2)) e0;;
(* EXPECTED: - : evT = Bool true *)

let s = CstInt(42) in
	let i = CstInt(42) in
		eval (IsSubset(s, i)) e0;;
(* EXPECTED: Exception: Failure "IsSubset: cannot run on non set types.". *)

let s = Of("int", [CstInt(3); CstInt(42); CstInt(5)]) in
	eval (Maximum(s)) e0;;
(* EXPECTED: - : evT = Int 42 *)

let s = Empty("int") in
	eval (Maximum(s)) e0;;
(* EXPECTED: Exception: Failure "Maximum: cannot run on empty sets.". *)

let s = CstInt(42) in
	eval (Maximum(s)) e0;;
(* EXPECTED: Exception: Failure "Maximum: cannot run on non set types.". *)

let s = Of("string", [EString("lorem") ; EString("ipsum") ; EString("dolor") ]) in
	eval (Minimum(s)) e0;;
(* EXPECTED: - : evT = String "dolor" *)

let s = Empty("int") in
	eval (Minimum(s)) e0;;
(* EXPECTED: Exception: Failure "Minimum: cannot run on empty sets.". *)

let s = CstInt(42) in
	eval (Minimum(s)) e0;;
(* EXPECTED: Exception: Failure "Minimum: cannot run on non set types.". *)

let s1 = Of("string", [EString("lorem") ; EString("ipsum") ; EString("dolor") ]) in
	let s2 = Of("string", [EString("ipsum") ; EString("sit") ; EString("amet") ]) in
		eval (Union(s1, s2)) e0;;
(* EXPECTED: - : evT = Set ("string", [String "lorem"; String "dolor"; String "amet"; String "sit"; String "ipsum"]) *)

let s1 = Of("string", [EString("lorem") ; EString("ipsum") ; EString("dolor") ]) in
	let s2 = Empty("int") in
		eval (Union(s1, s2)) e0;;
(* EXPECTED: Exception: Failure "Union: cannot run on set of different types.". *)

let s1 = Of("int", [CstInt(2); CstInt(42); CstInt(3)]) in
	let s2 = Empty("int") in
		eval (Union(s1, s2)) e0;;
(* EXPECTED: - : evT = Set ("int", [Int 3; Int 42; Int 2]) *)

let s1 = CstInt(42) in
	let s2 = Empty("int") in
		eval (Union(s1, s2)) e0;;
(* EXPECTED: Exception: Failure "Union: cannot run on non set types.". *)

let s1 = CstInt(42) in
	let s2 = CstInt(42) in
		eval (Union(s1, s2)) e0;;
(* EXPECTED: Exception: Failure "Union: cannot run on non set types.". *)

let s1 = Empty("int") in
	let s2 = Empty("int") in
		eval (Union(s1, s2)) e0;;
(* EXPECTED: - : evT = Set ("int", [Unbound]) *)

let s1 = Empty("bool") in
	let s2 = Empty("bool") in
		eval (Intersection(s1, s2)) e0;;
(* EXPECTED: - : evT = Set ("bool", [Unbound]) *)

let s1 = Of("int", [CstInt(2); CstInt(42); CstInt(3)]) in
	let s2 = Empty("bool") in
		eval (Intersection(s1, s2)) e0;;
(* EXPECTED: Exception: Failure "Intersection: cannot run on set of different types.". *)

let s1 = Of("int", [CstInt(2); CstInt(42); CstInt(3)]) in
	let s2 = Of("int", [CstInt(42)]) in
		eval (Intersection(s1, s2)) e0;;
(* EXPECTED: - : evT = Set ("int", [Int 42]) *)

let s1 = CstInt(42) in
	let s2 = Empty("int") in
		eval (Intersection(s1, s2)) e0;;
(* EXPECTED: Exception: Failure "Intersection: cannot run on non set types.". *)

let s1 = CstInt(42) in
	let s2 = CstInt(42) in
		eval (Intersection(s1, s2)) e0;;
(* EXPECTED: Exception: Failure "Intersection: cannot run on non set types.". *)

let s1 = Of("int", [CstInt(2); CstInt(42); CstInt(3)]) in
	let s2 = Of("int", [CstInt(42)]) in
		eval (Difference(s1, s2)) e0;;
(* EXPECTED: - : evT = Set ("int", [Int 2; Int 3]) *)

let s1 = Of("int", [CstInt(2); CstInt(42); CstInt(3)]) in
	let s2 = Of("bool", [CstTrue]) in
		eval (Difference(s1, s2)) e0;;
(* EXPECTED: Exception: Failure "Difference: cannot run on set of different types.". *)

let s1 = Of("bool", [CstTrue]) in
	let s2 = Of("bool", [CstTrue]) in
		eval (Difference(s1, s2)) e0;;
(* EXPECTED: - : evT = Set ("bool", [Unbound]) *)

let s1 = Empty("string") in
	let s2 = Empty("string") in
		eval (Difference(s1, s2)) e0;;
(* EXPECTED: - : evT = Set ("string", [Unbound]) *)

let s1 = CstInt(42) in
	let s2 = Empty("int") in
		eval (Difference(s1, s2)) e0;;
(* EXPECTED: Exception: Failure "Difference: cannot run on non set types.". *)

let s1 = CstInt(42) in
	let s2 = CstInt(42) in
		eval (Difference(s1, s2)) e0;;
(* EXPECTED: Exception: Failure "Difference: cannot run on non set types.". *)

let e1 = let f = Fun("n", Eq(Den "n", CstInt(6))) in bind e0 "f" (eval f e0) in
	let s = Of("int", [CstInt(2) ; CstInt(0) ; CstInt(42)]) in
		eval (For_all(Den "f", s)) e1;;
(* EXPECTED: - : evT = Bool false *)

let e1 = let f = Fun("n", Sum(Den "n", CstInt(6))) in bind e0 "f" (eval f e0) in
	let s = Of("int", [CstInt(2) ; CstInt(0) ; CstInt(42)]) in
		eval (For_all(Den "f", s)) e1;;
(* EXPECTED: Exception: Failure "For_all: function to be applied is not a predicate.". *)

let e1 = let f = Fun("n", Sum(Den "n", CstInt(6))) in bind e0 "f" (eval f e0) in
	let s = Empty("int") in
		eval (For_all(Den "f", s)) e1;;
(* EXPECTED: - : evT = Bool true *)

let e1 = let f = Fun("n", Eq(Den "n", CstInt(6))) in bind e0 "f" (eval f e0) in
	let s = CstInt(42) in
		eval (For_all(Den "f", s)) e1;;
(* EXPECTED: Exception: Failure "For_all: cannot run on non set types.". *)

let e1 = let f = Fun("n", Eq(Den "n", CstInt(6))) in bind e0 "f" (eval f e0) in
	let s = Of("int", [CstInt(2) ; CstInt(0) ; CstInt(42)]) in
		eval (Exists(Den "f", s)) e1;;
(* EXPECTED: - : evT = Bool false *)

let e1 = let f = Fun("n", Sum(Den "n", CstInt(6))) in bind e0 "f" (eval f e0) in
	let s = Of("int", [CstInt(2) ; CstInt(0) ; CstInt(42)]) in
		eval (Exists(Den "f", s)) e1;;
(* EXPECTED: Exception: Failure "Exists: function to be applied is not a predicate.". *)

let e1 = let f = Fun("n", Sum(Den "n", CstInt(6))) in bind e0 "f" (eval f e0) in
	let s = Empty("int") in
		eval (Exists(Den "f", s)) e1;;
(* EXPECTED: - : evT = Bool false *)

let e1 = let f = Fun("n", Eq(Den "n", CstInt(6))) in bind e0 "f" (eval f e0) in
	let s = CstInt(42) in
		eval (Exists(Den "f", s)) e1;;
(* EXPECTED: Exception: Failure "Exists: cannot run on non set types.". *)

let e1 = let f = Fun("n", Eq(Den "n", CstInt(6))) in bind e0 "f" (eval f e0) in
	let s = Of("int", [CstInt(2) ; CstInt(0) ; CstInt(42)]) in
		eval (Filter(Den "f", s)) e1;;
(* EXPECTED: - : evT = Set ("int", [Unbound]) *)

let e1 = let f = Fun("n", Eq(Times(Den "n", CstInt(0)), CstInt(0))) in bind e0 "f" (eval f e0) in
	let s = Of("int", [CstInt(2) ; CstInt(0) ; CstInt(42)]) in
		eval (Filter(Den "f", s)) e1;;
(* EXPECTED: - : evT = Set ("int", [Int 2; Int 0; Int 42]) *)

let e1 = let f = Fun("n", Sum(Den "n", CstInt(6))) in bind e0 "f" (eval f e0) in
	let s = Of("int", [CstInt(2) ; CstInt(0) ; CstInt(42)]) in
		eval (Filter(Den "f", s)) e1;;
(* EXPECTED: Exception: Failure "Filter: function to be applied is not a predicate.". *)

let e1 = let f = Fun("n", Eq(Den "n", CstInt(6))) in bind e0 "f" (eval f e0) in
	let s = CstInt(42) in
		eval (Filter(Den "f", s)) e1;;
(* EXPECTED: Exception: Failure "Filter: cannot run on non set types.". *)

let e1 = let f = Fun("n", Sum(Den "n", CstInt(6))) in bind e0 "f" (eval f e0) in
	let s = Empty("int") in
		eval (Map(Den "f", s)) e1;;
(* EXPECTED: Exception: Failure "Map: cannot determine resulting set type when run against empty sets.". *)

let e1 = let f = Fun("n", Eq(Den "n", CstInt(6))) in bind e0 "f" (eval f e0) in
	let s = CstInt(42) in
		eval (Map(Den "f", s)) e1;;
(* EXPECTED: Exception: Failure "Map: cannot run on non set types.". *)
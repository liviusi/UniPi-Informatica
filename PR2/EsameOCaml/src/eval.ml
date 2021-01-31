(* Linguaggio *)
type ide = string;;  (* identificatore *)

(* tipi denotabili *)
type exp = 
	| CstInt of int (* costante intera *)
	| CstTrue (* costante booleana true *)
	| CstFalse (* costante booleana false *)
	| EString of string
	| IsZero of exp
	| Or of exp * exp
	| And of exp * exp
	| Not of exp
	| Den of ide (* sostituisce a ide il suo valore *)
	| Sum of exp * exp (* somma tra interi *)
	| Sub of exp * exp (* sottrazione tra interi *)
	| Times of exp * exp (* moltiplicazione tra interi *)
	| Ifthenelse of exp * exp * exp (* operatore ternario ifthenelse *)
	| Eq of exp * exp (* equivalenza tra interi *)
	| Let of ide * exp * exp (* assegnamento *)
	| Fun of ide * exp (* astrazione di funzione unaria non ricorsiva *)
	| Letrec of ide * ide * exp * exp (* astrazione di funzione unaria ricorsiva *)
	| Apply of exp * exp (* applicazione di funzione *)
	(* -- INSIEMI -- *)
	| Empty of string (* costruisce un insieme vuoto *)
	| Singleton of exp * string (* costruisce un insieme di un solo elemento *)
	| Of of string * (exp list) (* costruisce un insieme di elementi *)
	(* operazioni di base *)
	| Add of exp * exp (* aggiunge (R) al set (L) *)
	| Remove of exp * exp (* rimuove (R) dal set (L) *)
	| IsEmpty of exp (* verifica che il set sia vuoto *)
	| BelongsTo of exp * exp (* verifica l'appartenenza di L al set R *)
	| IsSubset of exp * exp (* verifica che (L) sia un sottoinsieme di (R) *)
	| Minimum of exp (* restituisce l'elemento minimo del set *)
	| Maximum of exp (* restituisce l'elemento massimo del set *)
	| Union of exp * exp (* unione di due insiemi *)
	| Intersection of exp * exp (* intersezione di due insiemi *)
	| Difference of exp * exp (* differenza tra insiemi *)
	(* operatori funzionali *)
	| For_all of exp * exp (* verifica il predicato (L) su ogni elemento del set (R) *)
	| Exists of exp * exp (* verifica il predicato (L) su almeno un elemento del set (R) *)
	| Filter of exp * exp (* restituisce gli elementi del set (R) che verificano il predicato (L) *)
	| Map of exp * exp (* applica la funzione (L) a ogni elemento del set (R) *)
;;

(* Ambiente polimorfo *)
type 'v env = (string * 'v) list;;
(* tipi esprimibili *)
type evT =
	| Int of int
	| Bool of bool
	| String of string (* la specifica implica l'implementazione delle stringhe come tipo esprimibile *)
	| Set of string * (evT list) (* estensione per insiemi *)
	| Closure of ide * exp * evT env  (* chiusura *)
	| RecClosure of ide * ide * exp * evT env (* chiusura ricorsiva *)
	| Unbound
;;

let emptyEnv  = [ ("", Unbound)];;
(* EXPECTED: val emptyEnv : (string * evT) list = [("", Unbound)] *)

let bind (amb: evT env) (i:string) (x:evT) = ( i, x ) :: amb;; (* binding *)
(* EXPECTED: val bind : evT env -> string -> evT -> (string * evT) list = <fun> *)

let rec lookup (amb:evT env) (i:string) =
	match amb with (* ricerca i nell'ambiente amb *)
	| [] ->  Unbound
	| (j,v)::sl when j = i -> v
	| _::sl -> lookup sl i
;;
(*EXPECTED val lookup : evT env -> string -> evT = <fun> *)

let typecheck (x, y) : bool = (* string * evT *)
  match x with	(* typechecker *)
	| "int" -> 
		(match y with 
		| Int(_) -> true
		| _ -> false)
	| "bool" -> 
		(match y with 
		| Bool(_) -> true
		| _ -> false)
	| "string" -> (* aggiungo string al typecheck dinamico *)
		(match y with 
		| String(_) -> true
		| _ -> false)
	| "set" -> (* aggiungo set al typecheck dinamico *)
		(match y with 
		| Set(_) -> true
		| _ -> false)
	| _ -> failwith("Declared type has yet to be implemented.")
;;
(* val typecheck : string * evT -> bool = <fun> *)

(* interprete *)
let rec eval  (e : exp) (amb : evT env) =
	match e with
	| CstInt(n) -> Int(n)
	| CstTrue -> Bool(true)
	| CstFalse -> Bool(false)
	| EString(s) -> String(s)
	| Eq(e1, e2) -> int_eq( (eval e1 amb), (eval e2 amb) )
	| Times(e1,e2) -> int_times( (eval e1 amb), (eval e2 amb) )
	| Sum(e1, e2) -> int_plus( (eval e1 amb), (eval e2 amb) )
	| Sub(e1, e2) -> int_sub( (eval e1 amb), (eval e2 amb) )
	| IsZero(e) -> isZero( (eval e amb) )
	| Or(e1, e2) -> boolean_or( (eval e1 amb), (eval e2 amb) )
	| And(e1, e2) -> boolean_and( (eval e1 amb), (eval e2 amb) )
	| Not(e) -> boolean_not( (eval e amb) )
	| Ifthenelse(e1,e2,e3) ->
		let g = eval e1 amb in
		(
			match (typecheck("bool", g), g) with
			| (true, Bool(true)) -> eval e2 amb
			| (true, Bool(false)) -> eval e3 amb
			| (_, _) -> failwith ("Ifthenelese: cannot run on non boolean guard.")
		)
	| Den(i) -> lookup amb i
	| Let(i, e, ebody) -> eval ebody (bind amb i (eval e amb))
	| Fun(arg, ebody) -> Closure(arg,ebody,amb)
	| Letrec(f, arg, fbody, letBody) ->
		let benv = bind (amb) (f) (RecClosure(f, arg, fbody,amb)) in eval letBody benv
	| Apply(eF, eArg) ->
		let fclosure = eval eF amb in 
		(
			match fclosure with 
			| Closure(arg, fbody, fDecEnv) ->
				let aVal = eval eArg amb in
					let aenv = bind fDecEnv arg aVal in eval fbody aenv
			| RecClosure(f, arg, fbody, fDecEnv) ->
				let aVal = eval eArg amb in
					let rEnv = bind fDecEnv f fclosure in
						let aenv = bind rEnv arg aVal in eval fbody aenv
			| _ -> failwith("Apply: cannot apply non functional values.")
		)
	| Empty(aType) -> Set(aType, (evalSet aType [] amb))
	| Singleton(aElem, aType) -> 
		let eElem = eval aElem amb in
			if not (typecheck(aType, eElem)) then failwith("Singleton: element has mismatching type.")
			else Set(aType, [eElem])
	| Of(aType, elems) -> 
	(
		match elems with
		| [] -> eval (Empty(aType)) amb
		| [x] -> eval (Singleton(x, aType)) amb
		| x::y::xs -> Set(aType, (evalSet aType (x::y::xs) amb))
	)
	| Add(aSet, aElem) -> set_add( (eval aSet amb), (eval aElem amb) )
	| Remove(aSet, aElem) -> set_remove( (eval aSet amb), (eval aElem amb) )
	| IsEmpty(aSet) -> set_isempty( (eval aSet amb) )
	| BelongsTo(aSet, aElem) -> set_belongsto( (eval aSet amb), (eval aElem amb) )
	| IsSubset(aSet1, aSet2) -> set_issubset( (eval aSet1 amb), (eval aSet2 amb) )
	| Minimum(aSet) -> set_minimum( (eval aSet amb) )
	| Maximum(aSet) -> set_maximum( (eval aSet amb) )
	| Union(aSet1, aSet2) -> set_union( (eval aSet1 amb), (eval aSet2 amb) )
	| Intersection(aSet1, aSet2) -> set_intersection( (eval aSet1 amb), (eval aSet2 amb) )
	| Difference(aSet1, aSet2) -> set_difference( (eval aSet1 amb), (eval aSet2 amb) )
	(* Per le scelte implementative fatte nel caso degli operatori funzionali
	si consiglia di far riferimento alla relazione. *)
	| For_all(predicate, aSet) ->
	(
		match aSet with
		| Empty(setType) -> 
			let f = eval predicate amb in
			(
				match f with
				| Closure(_) -> Bool(true)
				| _ -> failwith("For_all: given predicate is not a function.")
			)
		| Of(str, elems) ->
		(
			match elems with
			| [] -> Bool(true)
			| x::xs ->
			(
				let tmp = (eval (Apply(predicate, x)) amb) in
					if (tmp = Bool(false)) then Bool(false)
					else if (typecheck("bool", tmp)) then (eval (For_all(predicate, (Of(str, xs)))) amb)
					else failwith("For_all: function to be applied is not a predicate.")
			)
		)
		| _ -> failwith("For_all: cannot run on non set types.")
	)
	| Exists(predicate, aSet) ->
	(
		match aSet with
		| Empty(setType) ->
			let f = eval predicate amb in
			(
				match f with
				| Closure(_) -> Bool(false)
				| _ -> failwith("For_all: given predicate is not a function.")
			)
		| Of(str, elems) ->
		(
			match elems with
			| [] -> Bool(false)
			| x::xs ->
			(
				let tmp = eval (Apply(predicate, x)) amb in
					if (tmp = Bool(true)) then Bool(true)
					else if (tmp = Bool(false)) then (eval (Exists(predicate, (Of(str, xs)))) amb)
					else failwith("Exists: function to be applied is not a predicate.")
			)
		)
		| _ -> failwith("Exists: cannot run on non set types.")
	)
	| Filter(predicate, aSet) ->
		let rec auxFilter predicate elems (res : exp list) : exp list =
		(
			match elems with
			| [] -> res
			| [x] when (eval x amb) = Unbound -> res
			| x::xs ->
			(
				let tmp = eval (Apply(predicate, x)) amb in
					if (tmp = Bool(true)) then auxFilter predicate xs (x::res)
					else if (tmp = Bool(false)) then auxFilter predicate xs res
					else failwith("Filter: function to be applied is not a predicate.")
			)
		) in
		(
			match aSet with
			| Empty(setType) -> (eval aSet amb)
			| Of(str, elems) ->
				let res = auxFilter predicate elems [] in
					if res=[] then eval (Empty(str)) amb
					else eval (Of(str, res)) amb
			| _ -> failwith("Filter: cannot run on non set types.")
		)
	| Map(func, aSet) ->
	(
		match aSet with
		| Empty(setType) -> failwith("Map: cannot determine resulting set type when run against empty sets.")
		| Of(setType, elems) ->
		(
			let getSetType (lis: evT list) : string =
			(
				let getType (value : evT) : string =
				(
					match value with
					| Int(_) -> "int"
					| String(_) -> "string"
					| Bool(_) -> "bool"
					| _ -> failwith("Map: function has invalid return type.") 
				) in
				let rec aux (lis: evT list) str : string =
				(
					match lis with
					| [] -> str
					| x::xs ->
						if ((getType x)<>str) then failwith("Map: cannot run against polymorphic function.")
						else aux xs str
				) in aux lis (getType (head lis))
			) in
			let map func elems =
				let rec aux func elems (res : evT list) =
				(
					match elems with
					| [] -> res
					| x::xs when not (member (eval (Apply(func, x)) amb) res) -> aux func xs ((eval (Apply(func, x)) amb)::res)
					| _::xs -> aux func xs res
				)
				in aux func elems [] in
					let tmp = map func elems in
						let str = getSetType tmp in Set(str, tmp)
		)
		| _ -> failwith("Map: cannot run on non set types.")
	)

(* verifica che la stringa setType sia un tipo valido per un insieme. *)
and validSetType setType : bool =
	match setType with
	| "int" -> true
	| "bool" -> true
	| "string" -> true
	| _ -> false

(* verifica che la coppia di parametri in input - valutata nell'ambiente -
possa rappresentare un insieme valido *)
and evalSet (aType : string) (aList : exp list) (amb : evT env) =
		let validSet setType eList = 
		let rec auxNoDuplicates eList =
			match eList with 
			| [] -> true
			| x::xs when (member x xs) -> false
			| _::xs -> auxNoDuplicates xs
		in let rec auxSameType s eList =
			match eList with
			| [] -> true
			| x::xs when (not (typecheck(s, x)) && x<>Unbound) -> false
			| _::xs -> auxSameType s xs
		in (auxNoDuplicates eList) && (auxSameType setType eList) && validSetType setType in
	if not (validSetType aType) then failwith("Set: invalid set type.")
	else 
	(
		let evalList (l : exp list) (amb : evT env) : (evT list) =
			let rec aux elist l amb = 
			match l with 
			| [] -> elist
			| x::xs -> aux ((eval x amb)::elist) xs amb
		in aux [] l amb in
			let eList = evalList aList amb in
				if not (validSet aType eList) then failwith("Set: set contains illegal elements (there may be duplicates and/or elements of different types).")
				else if eList=[] then [Unbound]
				else eList
	)

(* operazioni primitive *)
and int_eq(x,y) : evT =
	match (typecheck("int",x), typecheck("int",y), x, y) with
	| (true, true, Int(v), Int(w)) -> Bool(v = w)
	| (_,_,_,_) -> failwith("Eq: cannot check equality on non integer values.")

and int_plus(x, y) : evT =
	match(typecheck("int",x), typecheck("int",y), x, y) with
	| (true, true, Int(v), Int(w)) -> Int(v + w)
	| (_,_,_,_) -> failwith("Sum: cannot sum non integer values.")

and int_sub(x, y) : evT =
	match(typecheck("int",x), typecheck("int",y), x, y) with
	| (true, true, Int(v), Int(w)) -> Int(v - w)
	| (_,_,_,_) -> failwith("Sub: cannot subtract non integer values.")

and int_times(x, y) : evT =
	match(typecheck("int",x), typecheck("int",y), x, y) with
	| (true, true, Int(v), Int(w)) -> Int(v * w)
	| (_,_,_,_) -> failwith("Times: cannot multiply non integer values.")

and isZero(x) : evT =
	match(typecheck("int",x), x) with
	| (true, Int(v)) ->
		if (v=0) then Bool(true)
		else Bool(false)
	| (_,_) -> failwith("IsZero: cannot check whether non integer value is zero.")

and boolean_or(x, y) : evT =
	match (typecheck("bool", x), typecheck("bool", y), x, y) with
	| (true, true, Bool(v), Bool(w)) -> 
		if v then Bool(true)
		else Bool(w)
	| (_,_,_,_) -> failwith("Or: cannot apply logical or to non boolean types.")

and boolean_and(x, y) : evT =
	match (typecheck("bool", x), typecheck("bool", y), x, y) with
	| (true, true, Bool(v), Bool(w)) ->
		if (not v) then Bool(false)
		else Bool(w)
	| (_,_,_,_) -> failwith("And: cannot apply logical and to non boolean types.")

and boolean_not(x) : evT =
	match (typecheck("bool", x), x) with
	| (true, Bool(v)) ->
		if (not v) then Bool(true)
		else Bool(false)
	| (_,_) -> failwith("Not: cannot apply logical not to non boolean type.")

(* Funzione ausiliaria utilizzata per controllare che x appartenga a ls. *)
and member x ls =
	let rec aux x ls =
		match ls with
		| [] -> false
		| y::ys when y=x -> true
		| _::ys -> aux x ys
	in aux x ls

(* Funzione ausiliaria che restituisce il primo elemento di una lista non vuota. *)
and head s =
	match s with
	| [] -> failwith("Empty set does not have a head.")
	| x::xs -> x
(* Funzione ausiliaria che restituisce l'insieme formato da eSet U {eElem} a patto
che gli elementi dell'insieme siano dello stesso tipo di eElem. *)
and set_add(eSet, eElem) : evT =
	match eSet with
	| Set(str, set) ->
		if not (typecheck(str, eElem)) then failwith("Add: element of type to be added does not match set's.")
		else
		(
			match set with
			| [x] when x = Unbound -> Set(str, [eElem])
			| x::xs -> 
				if not (member eElem set) then Set(str, eElem::set)
				else eSet
			| _ -> failwith("Something has gone terribly wrong. You should never see this error message.")
		)
	| _ -> failwith("Add: cannot run on non set types.")
(* Funzione ausiliaria che restituisce l'insieme (eSet \ {eElem}). *)
and set_remove(eSet, eElem) : evT =
	let remove toDel lis =
		let rec auxRemove toDel lis res =
			match lis with
			| [] -> res
			| x::xs -> 
				if x = toDel then res@xs
				else auxRemove toDel xs (x::res)
	in auxRemove toDel lis [] in
	(
		match eSet with
		| Set(str, elems) ->
			if (not (typecheck(str, eElem)) || not (member eElem elems)) then eSet
			else Set(str, (remove eElem elems))
		| _ -> failwith("Remove: cannot run on non set types.")
	)
(* Funzione ausiliaria che restituisce il valore booleano (eSet==[Unbound]). *)
and set_isempty(eSet) : evT =
	match eSet with
	| Set(str, s) -> 
		if s<>[Unbound] then Bool(false)
		else Bool(true)
	| _ -> failwith("IsEmpty: cannot run on non set types.")

(* Funzione ausiliaria che restituisce il valore booleano (eElem is in eSet). *)
and set_belongsto(eSet, eElem) : evT =
	match eSet with
	| Set(str, elems) ->
		if not (member eElem elems) then Bool(false)
		else Bool(true)
	| _ -> failwith("BelongsTo: cannot run on non set types.")

(* Funzione ausiliaria che restituisce il valore booleano (eSet1 <contenuto_uguale> eSet2) *)
and set_issubset(eSet1, eSet2) : evT =
	match eSet1, eSet2 with
	| Set(str1, elems1), Set(str2, elems2) ->
	(
		let rec isSubset els1 els2 : evT =
			match els1 with
			| [] -> Bool(true)
			| x::xs ->
				if (not (member x els2) && x<>Unbound) then Bool(false)
				else isSubset xs els2 
		in 
			if str1=str2 then isSubset elems1 elems2
			else Bool(false)
	)
	| _ -> failwith("IsSubset: cannot run on non set types.")

(* Funzione ausiliaria che restituisce l'elemento minimo in un insieme *)
and set_minimum(eSet) : evT =
	let auxMinimum s =
		let rec aux min l =
			match l with
			| [] -> min
			| x::xs ->
			(
				if (min>x) then aux x xs
				else aux min xs
			)
		in aux (head s) s
	in match eSet with
	| Set(str, elems) ->
	(
		match elems with
		| x::xs when x<>Unbound -> auxMinimum elems
		| _ -> failwith("Minimum: cannot run on empty sets.")
	)
	| _ -> failwith("Minimum: cannot run on non set types.")

(* Funzione ausiliaria che restituisce l'elemento massimo in un insieme *)
and set_maximum(eSet) : evT =
	let auxMaximum s =
		let rec aux max l =
			match l with
			| [] -> max
			| x::xs ->
				if (x>max) then aux x xs
				else aux max xs
		in aux (head s) s
	in match eSet with
		| Set(str, elems) ->
		(
			match elems with
			| x::xs when x<>Unbound -> auxMaximum elems
			| _ -> failwith("Maximum: cannot run on empty sets.")
		)
		| _ -> failwith("Maximum: cannot run on non set types.")

(* Funzione ausiliaria che restituisce l'insieme (eSet1 U eSet2) a patto che i due insiemi
siano dello stesso tipo. *)
and set_union(eSet1, eSet2) : evT =
	let union (elems1 : evT list) (elems2 : evT list) =
		let rec aux (elems1 : evT list) (elems2 : evT list) (res : evT list) : evT list =
			match elems1 with
			| [] -> res@elems2
			| x::xs when not (member x elems2) -> aux xs elems2 (x::res)
			| _::xs -> aux xs elems2 res
	in 
		if (elems1 = [Unbound]) then elems2
		else if (elems2 = [Unbound]) then elems1
		else aux elems1 elems2 [] in
	(
		match (eSet1, eSet2) with
		| Set(str1, elems1), Set(str2, elems2) ->
				if not (str1=str2) then failwith("Union: cannot run on set of different types.")
				else Set(str1, (union elems1 elems2))
		| _ -> failwith("Union: cannot run on non set types.")
	)

(* Funzione ausiliaria che restituisce l'insieme (eSet1 ∩ eSet2) a patto che i due insiemi
siano dello stesso tipo. *)
and set_intersection(eSet1, eSet2) : evT =
	let intersection (elems1 : evT list) (elems2 : evT list) =
		let rec aux (elems1 : evT list) (elems2 : evT list) (res : evT list) : evT list =
			match elems1 with
			| [] -> res
			| x::xs when (member x elems2) -> aux xs elems2 (x::res)
			| _::xs -> aux xs elems2 res
	in aux elems1 elems2 [] in
	(
		match (eSet1, eSet2) with
		| (Set(str1, elems1), Set(str2, elems2)) ->
			if not (str1=str2) then failwith("Intersection: cannot run on set of different types.")
			else Set(str1, (intersection elems1 elems2))
		| _ -> failwith("Intersection: cannot run on non set types.")
	)

(* Funzione ausiliaria che restituisce l'insieme (eSet1 \ eSet2). *)
and set_difference(eSet1, eSet2) : evT =
	let difference (elems1 : evT list) (elems2 : evT list) =
		let rec aux (elems1 : evT list) (elems2 : evT list) (res : evT list) : evT list =
			match elems1 with
			| [] -> res
			| x::xs when not (member x elems2) -> aux xs elems2 (x::res)
			| _::xs -> aux xs elems2 res
		in aux elems1 elems2 [] in
		(
			match (eSet1, eSet2) with
			| (Set(str1, elems1), Set(str2, elems2)) ->
				let res = difference elems1 elems2 in
					if res<>[] then Set(str1, (difference elems1 elems2))
					else Set(str1, [Unbound])
			| _ -> failwith("Difference: cannot run on non set types.")
		)
;;
(* EXPECTED: 
val eval : exp -> evT env -> evT = <fun>
val validSetType : string -> bool = <fun>
val evalSet : string -> exp list -> evT env -> evT list = <fun>
val int_eq : evT * evT -> evT = <fun>
val int_plus : evT * evT -> evT = <fun>
val int_sub : evT * evT -> evT = <fun>
val int_times : evT * evT -> evT = <fun>
val isZero : evT -> evT = <fun>
val boolean_or : evT * evT -> evT = <fun>
val boolean_and : evT * evT -> evT = <fun>
val boolean_not : evT -> evT = <fun>
val member : evT -> evT list -> bool = <fun>
val head : evT list -> evT = <fun>
val set_add : evT * evT -> evT = <fun>
val set_remove : evT * evT -> evT = <fun>
val set_isempty : evT -> evT = <fun>
val set_belongsto : evT * evT -> evT = <fun>
val set_issubset : evT * evT -> evT = <fun>
val set_minimum : evT -> evT = <fun>
val set_maximum : evT -> evT = <fun>
val set_union : evT * evT -> evT = <fun>
val set_intersection : evT * evT -> evT = <fun>
val set_difference : evT * evT -> evT = <fun>
*)
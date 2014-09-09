/* animal.pro
  animal identification game.  

    start with ?- diagnose.     */

diagnose :- nl,
            write('* * * * * * * * * * * * * * * * *'),
            nl,
            write('*                               *'),
            nl,
            write('*         Dr. House             *'),
            nl,
            write('*     Diagnostic engine         *'),
            nl,
            write('*   -------------------------   *'),
            nl,
            write('*                               *'),
            nl,
            write('*   Created by:                 *'),
            nl,
            write('*   Dimitris Karakostas         *'),
            nl,
            write('*   Nikolas Stasinos            *'),
            nl,
            write('*                               *'),
            nl,
            write('* * * * * * * * * * * * * * * * *'),
            nl,
            nl,
			write('All answers must be followed by a dot(.)'),
            nl,
            write('Press undo. before starting the program in order to delete previously saved answers.'),
            nl,
            nl,
            hypothesize(Decease),
            write('Your decease is: '),
            write(Decease),
            nl,
            undo.

/* hypotheses to be tested */
hypothesize(cold)   :- cold, !.
hypothesize(flu)     :- flu, !.
hypothesize(pneumonia)   :- pneumonia, !.
hypothesize(unknown).             /* no diagnosis */

	 
cold :- (
		   (
             (
		      verify(cough),
		      verify(sore_throat)
		     )
		     ;
		     (
		      verify(cough),
		      verify(runny_nose)
		     )
		     ;
		     (
		      verify(sore_throat),
		      verify(runny_nose)
		     )
		   )
		   ,
		   (
		     verify(headache);
		     verify(muscle_pain);
		     verify(fatigue);
		     verify(loss_of_apetite)
		   )
		) 
		;
		(
		   verify(cough),
		   verify(sore_throat),
		   verify(runny_nose)
		)
		;
		(
		child
		,
		(
		 (
		  (
           (
		    verify(cough),
		    verify(fever)
		   )
		   ;
		   (
		    verify(fever),
		    verify(runny_nose)
		   )
		   ;
		   (
		    verify(fever),
		    verify(sore_throat)
		   )
		  )
		  ,
		  (
		   verify(headache);
		   verify(muscle_pain);
		   verify(fatigue);
		   verify(loss_of_apetite)
		  )
		 ) 
		 ;
		 (
		   verify(cough),
		   verify(fever),
		   verify(sore_throat),
		   verify(runny_nose)
		 )
		)
		).
		 
flu :-  (
		  verify(shivers),
		  verify(fever),
		  (
		     (
		      verify(fatigue),
		      verify(headache)
		     )
		     ;
		     (
		      verify(runny_nose),
		      verify(red_eyes)
		     )
		     ;
		     (
		      verify(fatigue),
		      verify(muscle_pain)
		     )
		     ;
		     (
		      verify(cough),
		      verify(nausea)
		     )
		  )
		).

pneumonia :- (
		      verify(cough),
              verify(shivers),
		      verify(fever)
			 )
		     ;
		     (
		      verify(cough),
		      verify(hard_breathing),
		      verify(chest_pain)
		     )
		     ;
		     (
		      verify(cough),
		      verify(chest_pain),
		      verify(increased_breathing)		 
		     )
		     ;
		     (
		      verify(shivers),
			  verify(fever),
		      verify(chest_pain),
		      verify(hard_breathing)
		     )
		     ;
		     (
		      verify(shivers),
			  verify(fever),
		      verify(chest_pain),
		      verify(increased_breathing)
		     ).
		 
child :- verify(age_less_than_13), !.


/* how to ask questions */
ask(Question) :-
    write('Do you have: '),
    write(Question),
    write('? '),
    read(Response),
    nl,
    ( (Response == yes ; Response == y ; Response == Yes ; Response == Y)
      ->
       assert(yes(Question)) ;
       assert(no(Question)), fail).

:- dynamic yes/1,no/1.

/* How to verify something */
verify(S) :-
   (yes(S) 
    ->
    true ;
    (no(S)
     ->
     fail ;
     ask(S))).

/* undo all yes/no assertions */
undo :- retract(yes(_)),fail. 
undo :- retract(no(_)),fail.
undo.
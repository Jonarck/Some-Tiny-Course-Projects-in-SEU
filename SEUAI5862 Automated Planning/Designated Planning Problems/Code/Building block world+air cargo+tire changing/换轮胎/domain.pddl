(define (domain change-tire) 
(:predicates (tire ?t) (at ?x ?y)) 

(:action remove 
 :parameters (?obj ?loc) 
 :precondition (at ?obj ?loc) 
 :effect (and (not (at ?obj ?loc)) 
              (at ?obj Ground)) )

(:action puton 
 :parameters (?t ?Axle) 
 :precondition (and (tire ?t) 
                    (at ?t Ground) 
                    (not (at Flat ?Axle))) 
 :effect (and (not (at ?t Ground)) 
              (at ?t ?Axle)) )

(:action leaveovernight 
 :effect (and (not (at Spare Ground)) 
              (not (at Spare Axle)) 
              (not (at Spare Trunk)) 
              (not (at Flat Ground)) 
              (not (at Flat Axle)) 
              (not (at Flat Trunk))) ))
(define (domain clean-room)
(:predicates (room ?r) (moppingrobot ?a) (clean ?r) (dirty ?r) (at ?a ?r) (near ?r1 ?r2))

(:action move
 :parameters (?a ?from ?to)
 :precondition (and (at ?a ?from)
                    (moppingrobot ?a)
                    (room ?from)
                    (room ?to)
                    (near ?from ?to))
 :effect (and (at ?a ?to)
              (not (at ?a ?from))))

(:action suck
 :parameters (?a ?p)
 :precondition (and (at ?a ?p)
                    (moppingrobot ?a)
                    (room ?p)
                    (dirty ?p) )
 :effect (and (clean ?p))))
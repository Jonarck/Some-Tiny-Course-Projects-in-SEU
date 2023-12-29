(define (domain air-transport)
(:predicates (plane ?p) (cargo ?c) (airport ?a) (at ?p ?a) (in ?c ?p))

(:action load
 :parameters (?c ?p ?a)
 :precondition (and (cargo ?c)
                    (plane ?p)
                    (airport ?a)
                    (at ?c ?a)
                    (at ?p ?a))
 :effect (and (not (at ?c ?a))
                   (in ?c ?p)) )

(:action unload
 :parameters (?c ?p ?a)
 :precondition (and (cargo ?c)
                    (plane ?p)
                    (airport ?a)
                    (in ?c ?p)
                    (at ?p ?a))
 :effect (and (at ?c ?a)
              (not (in ?c ?p))) )

(:action fly
 :parameters (?p ?from ?to)
 :precondition (and (airport ?from)
                    (airport ?to)
                    (plane ?p)
                    (at ?p ?from))
 :effect (and (not (at ?p ?from))
              (at ?p ?to))) )
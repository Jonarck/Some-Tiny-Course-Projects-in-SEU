(define (problem change-tire-problem) 
(:domain change-tire) 
(:objects Spare Flat Axle Trunk Ground) 
(:init (tire Flat) 
       (tire Spare) 
       (at Flat Axle) 
       (at Spare Trunk)) 
(:goal (at Spare Axle)) )
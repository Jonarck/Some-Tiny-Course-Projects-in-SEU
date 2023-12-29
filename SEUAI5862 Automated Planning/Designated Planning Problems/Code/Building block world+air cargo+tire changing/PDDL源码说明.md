# PDDL源码及说明

## 1. 积木世界

### （1）问题（规则）描述：

#### 	① 情景描述与规则归纳：

​			桌面上有正方形积木块，一块积木上只允许直接放置一块积木。 

- ​	一个机器人手臂能将积木挪动位置——置于桌面上或其他积木上。 
- ​	每次只能移动一块积木

#### 	② 对象提取：

​			两种对象：木块和桌面

### （2）初始状态：

![img](file:///C:\Users\JZY\AppData\Local\Temp\ksohtml15396\wps3.jpg)

### （3）目标状态：

![img](file:///C:\Users\JZY\AppData\Local\Temp\ksohtml15396\wps6.jpg)

### （4）动作：

#### A. 变量与常量：

​	a) 木块

​	b) 桌面

#### B. 谓词：

​	a) 【x为空】(clear ?b) 

​	b) 【x是桌面】(table ?t)

​	c) 【x是木块】(block ?b)

​	d) 【x在y上】 (on ?x ?y)

​	e) 【x等于y】(isequal ?x ?y))

### （5）伪代码：

> Init(On(A, Table) ∧ On(B, Table) ∧ On(C, A) ∧ Block (A) ∧ Block (B) ∧ Block (C) ∧ Clear (B) ∧ Clear (C)) Goal(On(A, B) ∧ On(B,C))
> Action(Move(b, x, y), 
> 	   PRECOND: On(b, x) ∧ Clear (b) ∧ Clear (y) ∧ Block (b) ∧ Block (y) ∧ (b \ne x) ∧ (b\ne y) ∧ (x\ne y), 
> 	   EFFECT: On(b, y) ∧ Clear (x) ∧ ¬On(b, x) ∧ ¬Clear (y)) 
> Action(MoveToTable(b, x), 
> 	   PRECOND: On(b, x) ∧ Clear (b) ∧ Block (b) ∧ (b\ne x), 
> 	   EFFECT: On(b, Table) ∧ Clear (x) ∧ ¬On(b, x))

### （6）源码：

#### A.Domain

```
(define (domain block-mover)
(:predicates (block ?b) (table ?t) (on ?x ?y) (clear ?b) (isequal ?x ?y)) 

(:action move 
 :parameters (?b ?x ?y) 
 :precondition (and (on ?b ?x) (clear ?b) (clear ?y) (block ?b) (block ?y) (not (isequal ?b ?x)) (not (isequal ?b ?y)) (not (isequal ?y ?x))) 
 :effect (and (on ?b ?y) (clear ?x) (not (on ?b ?x)) (not (clear ?y))) )

(:action movetotable 
 :parameters (?b ?x ?t) 
 :precondition (and (on ?b ?x) (clear ?b) (block ?b) (block ?x) (not (isequal ?x ?b)) (table ?t)) 
 :effect (and (on ?b ?t) (clear ?x) (not (on ?b ?x))) )
```

#### B.Problem

```
(define (problem block-mover-problem) 
	(:domain block-mover) 
	(:objects blocka blockb blockc tablet) 
	(:init (block blocka) (block blockb) (block blockc) (table tablet) (on blocka tablet) (on blockb tablet) (on blockc blocka) (clear blockb) (clear blockc)) 
	(:goal (and (on blocka blockb) (on blockb blockc)))
```

### （7）运行结果

#### 规划生成

```
(movetotable blockc blocka tablet)
(move blockb tablet blockc) 
(move blocka tablet blockb)
```



## 2.航空货运

### （1） 问题（规则）描述：

#### 	① 情景描述：

​	两架飞机在两个机场之间运送两件货物，两件货物初始分布于两个机场（起送地），要求用对应飞机将特定货物进行运输，是两件货物分别被运送至两个机场中对应的目的地。

#### 	② 对象提取：

​	三种对象：飞机、货物、机场

### （2） 初始状态：

​	货物C1、C2分别在机场SFO和JFK，飞机P1、P2分别停靠在SFO和JFK。 

### （3） 目标状态：

​	货物C1、C2分别在机场JFK和SFO。

### （4） 动作：

#### A. 变量与常量：

​	a) 货物

​	b) 飞机

​	c) 机场

#### B. 谓词（PDDL）：

​	a) 【x是飞机】(plane ?p) 

​	b) 【x是货物】(cargo ?c)

​	c) 【x是机场】(airport ?a)

​	d) 【飞机x在机场y】 (at ?p ?a) 

​	e) 【货物x在机场y】 (at ?c ?a) 

​	f) 【货物x在飞机y上】(in ?c ?p)

### （5）伪代码：

> %**初始状态**：货物C1、C2分别在机场SFO和JFK，飞机P1、P2分别停靠在SFO和JFK。 
>
> Init(At(C1, SFO) ∧ At(C2, JFK) ∧ At(P1, SFO) ∧ At(P2, JFK) ∧ Cargo(C1) ∧ Cargo(C2) ∧ 
>
> Plane(P1) ∧ Plane(P2) ∧ Airport(JFK) ∧ Airport(SFO)) 
>
> %**目标状态测试**：货物C1、C2分别在机场JFK和SFO。 
>
> Goal(At(C1, JFK) ∧ At(C2, SFO)) 
>
> %**行动1_装载**：在a装载c到p上，前提：货物c和飞机p都在机场a，结果：c在p上，不在a。 
>
> Action(Load(c, p, a), PRECOND: At(c, a) ∧ At(p, a) ∧ Cargo(c) ∧ Plane(p) ∧ Airport(a) EFFECT: 
>
> ¬At(c, a) ∧ In(c, p)) 
>
> %**行动2_卸载**：在a卸载p上的c，前提：货物c在飞机p上，p在机场a，结果：c不在p上，在a。 
>
> Action(Unload(c, p, a), PRECOND: In(c, p) ∧ At(p, a) ∧ Cargo(c) ∧ Plane(p) ∧ Airport(a) 
>
> EFFECT: At(c, a) ∧ ¬ In(c, p)) 
>
> %**行动3_飞行**：p从from飞to，前提：飞机p在机场from，to是机场，结果：p不在from，在to。 
>
> Action(Fly(p, from, to), PRECOND: At(p, from) ∧ Plane(p) ∧ Airport(from) ∧ Airport(to) 
>
> EFFECT: ¬ At(p, from) ∧ At(p, to)) 

### （6）源码：

#### A.domain

```
(define (domain air-transport) 
(:predicates (plane ?p) (cargo ?c) (airport ?a) (at ?p ?a) (in ?c ?p)) 

(:action load 
 :parameters (?c ?p ?a) 
 :precondition (and (cargo ?c) (plane ?p) (airport ?a) (at ?c ?a) (at ?p ?a)) 
 :effect (and (not (at ?c ?a))(in ?c ?p)) )
 
(:action unload 
 :parameters (?c ?p ?a) 
 :precondition (and (cargo ?c) (plane ?p) (airport ?a) (in ?c ?p) (at ?p ?a)) 
 :effect (and (at ?c ?a) (not (in ?c ?p))) )

(:action fly 
 :parameters (?p ?from ?to) 
 :precondition (and (airport ?from) (airport ?to) (plane ?p) (at ?p ?from)) 
 :effect (and (not (at ?p ?from)) (at ?p ?to)) ) )
```

#### B.problem

```
(define (problem air-transport-problem) 
(:domain air-transport) 
(:objects cargo1 cargo2 SFO JFK plane1 plane2)
(:init (plane plane1) (plane plane2) (airport SFO) (airport JFK) (cargo cargo1) (cargo cargo2) (at cargo1 SFO) (at cargo2 JFK) (at plane1 SFO) (at plane2 JFK)) 
(:goal (and (at cargo1 JFK) (at cargo2 SFO))) )
```

### （7）运行结果：

#### 规划生成

```
(load cargo2 plane2 jfk) 
(fly plane2 jfk sfo) 
(unload cargo2 plane2 sfo)
(load cargo1 plane2 sfo) 
(fly plane2 sfo jfk)
(unload cargo1 plane2 jfk)
```

## 3.换备胎

### （1） 问题（规则）描述：

#### ① 情景描述：

​	用后备箱中的备胎替换汽车的瘪轮胎，考虑三个动作：

- l 卸掉轮胎： 
- l 安装轮胎：
- l 汽车无人值守过夜：全部轮胎会被盗走。

#### ② 对象提取：

​	两种对象：轮胎、轮胎去向

### （2） 初始状态：

​	轮胎在轮轴上，备胎在后备箱

### （3） 目标状态：

​	备胎在轮轴上

### （4） 动作：

#### ① 动作分解与分类：

​	两个动作：卸载轮胎与安装轮胎

​	一个结果性动作：汽车无人值守过夜，全部轮胎会被盗走。

#### ② 动作函数构建：

#### A. 变量与常量：

​	a) 轮轴

​	b) 后备箱

​	c) 轮胎

​	d) 地面

#### B. 谓词（PDDL）：

​	a) 【x是轮胎】(tire ?t) 

​	b) 【轮胎x在y上（中）】(at ?x ?y) 

### （5）伪代码：

> Init(Tire(Flat) ∧ Tire(Spare) ∧ At(Flat, Axle) ∧ At(Spare, Trunk)) 
>
> Goal(At(Spare, Axle)) 
>
> Action(Remove(obj , loc), 
>
> PRECOND: At(obj , loc) 
>
> EFFECT: ¬ At(obj , loc) ∧ At(obj, Ground)) 
>
> Action(PutOn(t, Axle), 
>
> PRECOND: Tire(t) ∧ At(t, Ground) ∧ ¬ At(Flat, Axle) 
>
> EFFECT: ¬ At(t, Ground) ∧ At(t, Axle)) 
>
> Action(LeaveOvernight, 
>
> PRECOND: 
>
> EFFECT: ¬ At(Spare, Ground) ∧ ¬ At(Spare, Axle) ∧ ¬ At(Spare, 
>
> Trunk)∧ ¬ At(Flat, Ground) ∧ ¬ At(Flat, Axle) ∧ ¬ At(Flat, Trunk))

### （6）源码：

#### A.domain

```
(define (domain change-tire) 
(:predicates (tire ?t) (at ?x ?y))

(:action remove 
 :parameters (?obj ?loc) 
 :precondition (at ?obj ?loc) 
 :effect (and (not (at ?obj ?loc)) (at ?obj Ground)) )
 
(:action puton 
 :parameters (?t ?Axle) 
 :precondition (and (tire ?t) (at ?t Ground) (not (at Flat ?Axle))) 
 :effect (and (not (at ?t Ground)) (at ?t ?Axle)) ) 
 
(:action leaveovernight 
 :effect (and (not (at Spare Ground)) (not (at Spare Axle)) (not (at Spare Trunk)) (not (at Flat Ground)) (not (at Flat Axle)) (not (at Flat Trunk))) ) )
```

#### B.problem

```
(define (problem change-tire-problem)
(:domain change-tire) 
(:objects Spare Flat Axle Trunk Ground)
(:init (tire Flat) (tire Spare) (at Flat Axle) (at Spare Trunk)) 
(:goal (at Spare Axle)) )
```

### （7）运行结果：

#### 规划生成

```
(remove flat axle) 
(remove spare trunk) 
(puton spare axle)
```


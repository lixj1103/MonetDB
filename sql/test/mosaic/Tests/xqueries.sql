set optimizer='sequential_pipe';

create table tmp3( i integer, b boolean, f real,t timestamp);
insert into tmp3 values
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(2, false, 0.314,'2014-08-23 11:34:54.000000'),
(3, true, 0.314,'2014-08-23 11:34:54.000000'),
(2, false, 0.314,'2014-08-23 11:34:54.000000'),
(3, true, 0.314,'2014-08-23 11:34:54.000000'),
(2, false, 0.314,'2014-08-23 11:34:54.000000'),
(3, true, 0.314,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(0, false, 0.316,'2014-08-23 11:34:54.000000'),
(5, false, 0.317,'2014-08-23 11:34:54.000000'),
(6, false, 0.317,'2014-08-23 11:34:54.000000'),
(7, false, 0.317,'2014-08-23 11:34:54.000000'),
(8, false, 0.317,'2014-08-23 11:34:54.000000'),
(9, false, 0.317,'2014-08-23 11:34:54.000000'),
(10, false, 0.317,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(1, true, 0.314,'2014-08-23 11:34:54.000000'),
(2, false, 0.314,'2014-08-23 11:34:54.000000'),
(3, true, 0.314,'2014-08-23 11:34:54.000000'),
(2, false, 0.314,'2014-08-23 11:34:54.000000'),
(3, true, 0.314,'2014-08-23 11:34:54.000000'),
(2, false, 0.314,'2014-08-23 11:34:54.000000'),
(3, true, 0.314,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(4, true, 0.316,'2014-08-23 11:34:54.000000'),
(0, false, 0.316,'2014-08-23 11:34:54.000000'),
(5, false, 0.317,'2014-08-23 11:34:54.000000'),
(6, false, 0.317,'2014-08-23 11:34:54.000000'),
(7, false, 0.317,'2014-08-23 11:34:54.000000'),
(8, false, 0.317,'2014-08-23 11:34:54.000000'),
(9, false, 0.317,'2014-08-23 11:34:54.000000'),
(10, false, 0.317,'2014-08-23 11:34:54.000000');

alter table tmp3 set read only;
select sum(i) from tmp3;
select sum(f) from tmp3;

explain select count(*) from tmp3 where i <4;
select count(*) from tmp3 where i <4;
explain select count(*) from tmp3 where i = 7;
select count(*) from tmp3 where i = 7;
select count(*) from tmp3 where f <0.316;
select count(*) from tmp3 where b = true;

alter table tmp3 alter column i set storage 'literal';
alter table tmp3 alter column b set storage 'literal';
alter table tmp3 alter column f set storage 'literal';
select sum(i) from tmp3;
select sum(f) from tmp3;

explain select count(*) from tmp3 where i <4;
select count(*) from tmp3 where i <4;
explain select count(*) from tmp3 where i = 7;
select count(*) from tmp3 where i = 7;
select count(*) from tmp3 where f <0.316;
select count(*) from tmp3 where b = true;

--select * from storage where "table" = 'tmp3';
alter table tmp3 alter column i set storage NULL;
alter table tmp3 alter column b set storage NULL;
alter table tmp3 alter column f set storage NULL;

explain select count(*) from tmp3 where i <4;
select count(*) from tmp3 where i <4;
explain select count(*) from tmp3 where i = 7;
select count(*) from tmp3 where i = 7;
select count(*) from tmp3 where f <0.316;
select count(*) from tmp3 where b = true;

alter table tmp3 set read write;
--select * from tmp3;

drop table tmp3;
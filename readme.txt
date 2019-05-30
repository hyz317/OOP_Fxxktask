E 数据导入导出 byz 
导入：括号里面的是field
输出：不输出表头 直接输出一个表的数据
ifstream fin("byztql.txt");
ofstream fout("fxxkoop.txt");
fin>>x;
fout<<x;
E count wtr

N group by wtr
count + group by => 输出 group by 选中列中的每个信息各有几个
count => 直接输出符合的行的数量
H order by hyz
order by 默认升序

E 多表whereclause hyz
E union byz
可以用 set<Row*> s; (insert,返回值是一个pair<set::iterator, bool>，可以用auto)
需要重新整一个Rule（比较指针所指向的地方是否相等，不是指针的地址相等）
union all 用 multiset
输出和 select 有关，和 union 无关
E 数字函数 wtr
E（麻烦） 扩展数据类型DATE和TIME PIGEON
E 算数运算符 hyz
H Join hyz
X（耗时间）数据库存档 byz
E 1、直接记录所有输入语句 (白嫖？？？) 5%
fE 2、通过现有数据反解出 5% （nmd我要把你删了） N 3、线性存储到txt中 10%
EH 4、心里有B数的存储到txt中 20%
（细节继续商讨） fN like
H socket wtr
+云端服务器 hyz

拓展功能1 AST hyz
拓展功能2 算数运算符原位修改数据

=====时间表=====

hyz: Mon 15:05-22:00 Tue 15:05-22:00 Wed Pigeon Thu 全天 Fri 9:50-18:00 Sat Sun

byz: Mon: 9:35-15:00 Tue: 15:05-22:00 Wed: 12:00-19:00 Thu: 12:00-19:00 Fri 13:30-19:00 AI BID

wtr: Mon 15:05-22:00 Tue: 15:05-22:00 Wed: 15:05-22:00 Thu: 13:00-19:00 Fri: 13:00-? AI
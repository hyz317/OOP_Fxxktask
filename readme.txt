E ���ݵ��뵼�� byz 
���룺�����������field
������������ͷ ֱ�����һ���������
ifstream fin("byztql.txt");
ofstream fout("fxxkoop.txt");
fin>>x;
fout<<x;
E count wtr

N group by wtr
count + group by => ��� group by ѡ�����е�ÿ����Ϣ���м���
count => ֱ��������ϵ��е�����
H order by hyz
order by Ĭ������

E ���whereclause hyz
E union byz
������ set<Row*> s; (insert,����ֵ��һ��pair<set::iterator, bool>��������auto)
��Ҫ������һ��Rule���Ƚ�ָ����ָ��ĵط��Ƿ���ȣ�����ָ��ĵ�ַ��ȣ�
union all �� multiset
����� select �йأ��� union �޹�
E ���ֺ��� wtr
E���鷳�� ��չ��������DATE��TIME PIGEON
E ��������� hyz
H Join hyz
X����ʱ�䣩���ݿ�浵 byz
E 1��ֱ�Ӽ�¼����������� (���Σ�����) 5%
fE 2��ͨ���������ݷ���� 5% ��nmd��Ҫ����ɾ�ˣ� N 3�����Դ洢��txt�� 10%
EH 4��������B���Ĵ洢��txt�� 20%
��ϸ�ڼ������֣� fN like
H socket wtr
+�ƶ˷����� hyz

��չ����1 AST hyz
��չ����2 ���������ԭλ�޸�����

=====ʱ���=====

hyz: Mon 15:05-22:00 Tue 15:05-22:00 Wed Pigeon Thu ȫ�� Fri 9:50-18:00 Sat Sun

byz: Mon: 9:35-15:00 Tue: 15:05-22:00 Wed: 12:00-19:00 Thu: 12:00-19:00 Fri 13:30-19:00 AI BID

wtr: Mon 15:05-22:00 Tue: 15:05-22:00 Wed: 15:05-22:00 Thu: 13:00-19:00 Fri: 13:00-? AI
# interpreter
一个对输入的字符串进行解析的类，支持复数，矩阵，数组运算，目前有32种通用函数可在字符串中使用
字符串的格式如下
a=5+sin(pi)^2+cos(6+2*pi)/2;
b=[5,6,7,8;1,2,3,4];
c=a*b;

例程的测试命令
  help            帮助程序
  clc             清空输入
  show val        显示已定义变量
  clear           清除已定义变量
  test            测试模式
  read            读取测试文件
  show func       显示所有函数名
  show opre       显示所有运算符
  
  在这个解释器中，所有的字符串都可以拆分为一个个单元，每个单元都有自己的优先级，解释器依照每个单元的优先级依次从大到小
对每个单元执行相应的操作并删除单元。因此，当单元数减至1时就代表了解释器完成了工作。
语法规则如下：
  数据类型：
    目前基础数据类型为6种：
      实数
      布尔类型
      复数
      数组
      实数矩阵
      复数矩阵
    所有的数据类型在运行过程中都会被打包成Base_Data类型进行传递。
    关于数组类型的几点说明：
    1.数组单元内容的基础数据类型可以不一致
    2.数组的作用主要为对数据的批量计算，如下式
      a=(1,1+2,1-5);
      a-2;
      计算得出的ans=(-1,1,-6)
  单元种类：
  在这个解释器中，单元细分可分为12种：
  BINARY_OPERATOR：
    二元操作符节点，由下列字符产生
    +  -  *  /  %  ^  >  <
    
    这些字符特点为单个字符，并且操作数为2个，用于对操作数运算
    
  STATIC_FUNCTION
    静态函数节点，由指定的字符序列产生如：
    sin  cos  tan
    这些函数的特点为单变量，单返回值，当出现多变量时，多个变量会组成一个数组，作为单个变量传入函数中，用于对操作数运算
    
  UNARY_OPERATOR
    一元操作符节点，由以下字符产生
    !   `
    这些操作符的特点为单个字符，并且操作数为1，均为左结合性，用于对操作数运算
    
  ASSIGNMENT_OPERATOR
    赋值运算节点，仅由这个字符产生
    =
    操作数为2，用于产生新的变量或更改旧的变量的值（注：变量产生的唯一方法）
    变量命名规则与C语言同
    左值：变量或未定义字符
    右值：变量或常量
  NUM
    普通的数值节点，由以下数字或数值常量产生，如
    1 2 3 4 5 6 7 8 9 0 . pi i j e ans（代表上一次计算结果的常量）
    用于产生值常量
  
  NUM_ELEME
    数值标签节点，由已定义变量和标签产生，如
    a=(5,6,7,8);
    b=[1,2,3;4,5,6];
    c=[1 2;3 4];
    a(1)          //获取数组a的第二个元素
    b(1,2)        //获取矩阵b的第二行，第三列元素
    c(0)          //获取矩阵c的第一行行向量
    节点保存主变量地址以及偏移量
    
  NUM_VARIBLE
    数值变量节点，由已定义变量产生，如
    a1  b2 c_3 aaa
  
  BRACKET_OPERATOR
    括号节点，由一对完整的
    ()
    产生
    括号内的节点优先于括号外的节点化简
    
  SQ_BEACKET_OPERATOR
    方括号节点，由一对完整的
    []
    产生
    括号内的节点优先于括号外的节点化简，用于定义矩阵，如
    a=[1 2 3 4;5 6 7 8] //a赋值为2*4的矩阵
    b=[6,7,8,9;5,4 3 2] //b赋值为2*4的矩阵
    
  DIVIDE_OPERATOR
    分隔符节点，用于在方括号节点中，作为一行的结束符，如
    （方括号内的）;
  
  COMMA_OPRERATOR
    逗号运算符，用于将两侧的元素合称为一个数组，如
    a=(5,6,7,8,9)   //a赋值为5个元素的数组
    sin(a)          //对数组运算，结果仍然为数组
    
  UNDEF_NAME
    未定义名称的节点，用于作为赋值运算符的左操作数
  
  附录 
  各种符号的优先级：
    数值或变量         0
    ,                 1
    =                 2
    >                 3
    <                 3
    +                 4
    -                 4
    *                 5
    /                 5
    %                 5
    ^                 6
    函数              7
    !                 8
    `                 8
    ()                9
    []                9
    
  优先级高的优先执行，优先级相同则按照表达式中从左到右顺序
  

  
    

## 变量、var与let

JavaScript 的**变量命名**有两个限制：

1. 变量名称必须仅包含字母，数字，符号 `$` 和 `_`。
2. 首字符必须非数字。



声明一个常数（不变）变量，可以使用 `const` 而非 `let`，`const` — 类似于 `let`，但是变量的值无法被修改。

```javascript
const PI = 3.14;
```



`var` 与 `let/const` 有两个主要的区别：

1. `var` 声明的变量**没有块级作用域**，它们仅在当前**函数内**可见，或者**全局**可见（如果变量是在函数外声明的）。
2. `var` 变量声明在**函数开头**就会被处理（声明提前，脚本启动对应全局变量）。

另外`var`还允许重新声明，而`let`在同一个作用域下将一个变量声明2次将会导致错误。



## 原始类型及其方法

https://zh.javascript.info/primitives-methods （有意思）

- 在 JavaScript 中有 7 种原始类型：`string`，`number`，`bigint`，`boolean`，`symbol`，`null` 和 `undefined`。

**原始类型不是对象**，注意上面的类型名都是小写字母开头



以下是 JavaScript 创建者面临的悖论：

- 人们可能想对诸如字符串或数字之类的原始类型执行很多操作。最好将它们**作为方法来访问**。
- 原始类型**必须尽可能的简单轻量**。

而解决方案看起来多少有点尴尬，如下：

1. 原始类型**仍然是原始的**。与预期相同，提供单个值
2. JavaScript **允许访问**字符串，数字，布尔值和 symbol 的**方法和属性**。
3. 为了使它们起作用，创建了提供额外功能的特殊**“对象包装器”**，**使用后即被销毁**。

“对象包装器”对于每种原始类型都是不同的，它们被称为 `String`、`Number`、`Boolean` 和 `Symbol`。因此，它们提供了不同的方法。

用法演示如下：

```javascript
let str = "Hello";

alert( str.toUpperCase() ); // HELLO
```

很简单，对吧？以下是 `str.toUpperCase()` 中**实际发生的情况**：

1. 字符串 `str` 是一个原始值。因此，在访问其属性时，**会创建一个包含字符串字面值的特殊对象**，并且**具有有用的方法**，例如 `toUpperCase()`。
2. 该方法运行并返回一个新的字符串（由 `alert` 显示）。
3. **特殊对象被销毁**，只留下原始值 `str`。



### **总结**

- 除 `null` 和 `undefined` 以外的原始类型都提供了许多有用的方法。
- 从形式上讲，这些方法**通过临时对象工作**，但 JavaScript 引擎可以很好地调整，以在内部对其进行优化，因此调用它们并不需要太高的成本。



## 类型装换

**Number**

四则运算 会自动将值转换成number类型

加号，可以作为四则运算的加法运算符，也可以作为“连字符”，加号会优先选择连字符

| `undefined`     | `NaN`                                                        |
| --------------- | ------------------------------------------------------------ |
| `null`          | `0`                                                          |
| `true 和 false` | `1` and `0`                                                  |
| `string`        | 去掉首尾空格后的纯数字字符串中含有的数字。如果剩余字符串为空，则转换结果为 `0`。否则，将会从剩余字符串中“读取”数字。当类型转换出现 error 时返回 `NaN`。 |

```javascript
alert( Number("   123   ") ); // 123
alert( Number("123z") );      // NaN（从字符串“读取”数字，读到 "z" 时出现错误）
alert( Number(true) );        // 1
alert( Number(false) );       // 0
```



**Boolean**

转换发生在进行逻辑操作时，也可以通过 `Boolean(value)` 进行显式转换。

- 直观上为“空”的值（如 `0`、空字符串、`null`、`undefined` 和 `NaN`）将变为 `false`。
- 其他值变成 `true`。





## 箭头函数

1.不带花括号：`(...args)=>expression`，右侧是一个表达式：函数计算表达式并返回其结果

2.带花括号：`(..args)=>{body}`，花括号运行在函数中编写多条语句，但是需要显示return来返回一些内容

```javascript
let sum = (x, y) => x + y;

let sum = (x, y) => {
	let result = x + y;
	return result;
}

let say_hi = () => alert('Hi'); //没有参数
```



## 构造函数

https://zh.javascript.info/constructor-new

在技术上是常规函数，有两个**约定**：

1.它们的命名以**大写字母开头**

2.他们**只能由“new”操作符**来执行

```javascript
//常规创建函数的方式
let user = {
  name: "Jack",
  isAdmin: false
};

//构造函数
function User(name) {
  this.name = name;
  this.isAdmin = false;
}

//构造函数的行为
function User(name) {
  // this = {};（隐式创建）

  // 添加属性到 this
  this.name = name;
  this.isAdmin = false;

  // return this;（隐式返回）
}

let a = new A; //如果构造函数A没有参数
```




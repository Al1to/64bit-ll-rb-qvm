# 64-bit Low-Level Register-based Quantum Virtual Machine
###### 64-битная Низкоуровневая Регистровая Квантовая Виртуальная Машина

НЕДОПИСАНО

### О проекте
Квантовая виртуальная машина была написана по приколу. Но она работает! 
<br/>Я прекрасно понимаю, что у нее есть кучу недостатков и недоделанных мест, но если пытаться это все исправлять, то лучше будет начинать с нуля. Возможно, может быть, если повезет, когда-нибудь я это сделаю.
<br/>*P.S. Комментариев в коде нет :')*
<br/>
### Навигация
 * [**Регистры**](#regs)
   * [Системные](#sregs)
   * [Классические *+опкоды*](#cregs)
   * [Квантовые](#qregs)
 * [**Память**](#mem)
   * [Программируемая](#mmem)
   * [Стек](#smem)
   * [Таблица меток](#ltmem)
   * [Таблица переменных](#dtmem)
 * [**Кубиты**](#qubits)
   * [Принцип работы](#qubitoppr)
   * [Реализованные операции](#qubits2)
 * [**Свой язык ассемблера (QASM)**](#qasm)
   * [Синтаксис](#qasm_syntax)
   * [Инструкции](#qasm_instrs)
     * [Стандартные *+опкоды*](#qasm_cinstrs)
     * [Квантовые *+опкоды*](#qasm_qinstrs)
   * [Метки](#qasm_lbls)
   * [Переменные](#qasm_datas)
     * [Косплей на массивы](#qasm_arrays)
 * [**Сообщения об ошибках**](#errs)
   * [Ошибки неверных параметров запуска](#errs1)
   * [Ошибки синтаксиса](#errs2)
   * [Непредусмотренные виды ошибок](#errs3)
 * [**Примеры простых программ**](#1prgs)
   * [1](#1prg1)
   * [2](#1prg2)
   * [3](#1prg3)
 * [**Можно и посложнее**](#2prgs)
   * [1](#2prgs1)
<br/>
<br/>

<a id="regs"></a>
## Регистры

<a id="sregs"></a>
#### Системные регистры
Я их выделил в отдельную категорию, т.к. с ними нельзя взаимодействовать в коде, они нужны исключительно для работы виртуальной машины.
 - `prgc` *(англ. program counter)* - удерживает адрес следующей команды, которая будет выполняться процессором. prgc указывает на текущую позицию в коде программы и увеличивается после каждой выполненной команды. Он обеспечивает последовательное выполнение команд, определяя порядок, в котором выполняются инструкции. Также, инструкции условных прыжков для своей работы просто меняют его значение.
 - `sp` *(англ. stack pointer)* - удерживает адрес текущей вершины стека, где хранятся локальные переменные и параметры функций.
 - `fp` *(англ. frame pointer)* - удерживает адрес начала текущего фрейма стека. Фрейм стека представляет собой область памяти, выделенную для отдельного вызова функции.
<br/>

<a id="cregs"></a>
#### Классические регистры
Классические регистры в моей виртуальной машине организованы в иерархическую структуру, где каждый регистр является подмножеством более крупного регистра. Эта структура позволяет использовать регистры разного размера в зависимости от требований выполняемой задачи.
<br/>Например, регистр `eax` является 32-битной частью 64-битного регистра `rax`. Это означает, что при выполнении 32-битной операции можно использовать регистр `eax`, а при выполнении 64-битной операции необходимо использовать регистр `rax`.
Или, например, регистры `ch` и `cl` являются первым и вторым байтом регистра `cx` соответственно.
<br/>Использование классических регистров аналогично использованию регистров в других ЭВМ. Они используются для хранения временных данных, результатов вычислений, адресов памяти и других значений. Иерархическая структура регистров позволяет оптимизировать использование памяти, выбирая регистр соответствующего размера для конкретной задачи.
###### Вот таблица всех класических регистров:
<table>
  <thead>
    <tr>
      <th>Опкод</th>
      <th>Название</th>
      <th>Размер, бит</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>0x5A</td>
      <td>rax</td>
      <td>64</td>
    </tr>
    <tr>
      <td>0x5B</td>
      <td>rbx</td>
      <td>64</td>
    </tr>
    <tr>
      <td>0x5C</td>
      <td>rcx</td>
      <td>64</td>
    </tr>
    <tr>
      <td>0x5D</td>
      <td>rdx</td>
      <td>64</td>
    </tr>
    <tr>
      <td>0x4A</td>
      <td>eax</td>
      <td>32</td>
    </tr>
    <tr>
      <td>0x4B</td>
      <td>ebx</td>
      <td>32</td>
    </tr>
    <tr>
      <td>0x4C</td>
      <td>ecx</td>
      <td>32</td>
    </tr>
    <tr>
      <td>0x4D</td>
      <td>edx</td>
      <td>32</td>
    </tr>
    <tr>
      <td>0x3A</td>
      <td>ax</td>
      <td>16</td>
    </tr>
    <tr>
      <td>0x3B</td>
      <td>bx</td>
      <td>16</td>
    </tr>
    <tr>
      <td>0x3C</td>
      <td>cx</td>
      <td>16</td>
    </tr>
    <tr>
      <td>0x3D</td>
      <td>dx</td>
      <td>16</td>
    </tr>
    <tr>
      <td>0x2A</td>
      <td>ah</td>
      <td>8</td>
    </tr>
    <tr>
      <td>0x2B</td>
      <td>bh</td>
      <td>8</td>
    </tr>
    <tr>
      <td>0x2C</td>
      <td>ch</td>
      <td>8</td>
    </tr>
    <tr>
      <td>0x2D</td>
      <td>dh</td>
      <td>8</td>
    </tr>
    <tr>
      <td>0x1A</td>
      <td>al</td>
      <td>8</td>
    </tr>
    <tr>
      <td>0x1B</td>
      <td>bl</td>
      <td>8</td>
    </tr>
    <tr>
      <td>0x1C</td>
      <td>cl</td>
      <td>8</td>
    </tr>
    <tr>
      <td>0x1D</td>
      <td>dl</td>
      <td>8</td>
    </tr>
  </tbody>
</table>
<br/>

<a id="qregs"></a>
#### Квантовые регистры
В отличии от классических, квантовые регистры изначально не объявлены, они создаются по ходу выполнения программы при помощи [инструкции **qreg**](#qasm_qinstrs_qreg).
Такие регистры имеют свои собственные имена и хранят [кубиты](#qubits) в количестве от 1 до 255. Все это задается при создании квантового регистра. 
<br/>Рекомендую воспринимать квантовый регистр как массив кубитов, к которому можно обращаться по индексу для доступа к кубиту.
<br/>Также стоит сказать, что виртуальная машина конвертирует все имена квантовых регистров (а еще [переменных](#qasm_datas) и [меток](#qasm_lbls)) в двухбайтовые коды, а следовательно количество квантовых регистров не может быть больше 65536. (Кому вообще столько их пригодится?)
И нет, коды переменных, меток и квантовых регистров никак не связаны, например, одновременно могут существовать все эти три штуки с кодом 0x0001 и все будет исправно работать.
<br/>
<br/>

<a id="mem"></a>
## Память
// TODO:

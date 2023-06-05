#ifndef _INPUT_H_
#define _INPUT_H_

struct Input
{
  bool Left;
  bool Right;
  bool Up;
  bool Down;

  Input()
  {
    Up = Down = Left = Right = false;
  }

  /*void Pack(ZCom_BitStream &str) {
    str.addBool(Left);
    str.addBool(Right);
    str.addBool(Up);
    str.addBool(Down);
  }

  void Unpack(ZCom_BitStream &str) {
    Left = str.getBool();
    Right = str.getBool();
    Up = str.getBool();
    Down = str.getBool();
  }*/

  bool operator==(const Input &in)
  {
    return (Up == in.Up && Down == in.Down && Right == in.Right && Left == in.Left);
  }

  Input &operator=(const Input &rhs)
  {
    Up = rhs.Up;
    Down = rhs.Down;
    Left = rhs.Left;
    Right = rhs.Right;
    return *this;
  }
};

#endif /* _INPUT_H_ */

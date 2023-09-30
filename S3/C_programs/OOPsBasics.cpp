
#include <stdio.h>

class Painter
{
  protected:
    char* name;

  public:
    Painter(char* name) //the constructor has to have the same name as the class
    {
      this->name = name;
    }

    virtual void Paint()
    {
      printf("The %s is painting currently\n", this->name);
    }    
};

class DoorPainter: public Painter
{
  public:
    DoorPainter(char* name) : Painter(name) //the constructor has to have the same name as the class
    {
    }

    void Paint() override
    {
      printf("The %s is painting door currently\n", this->name);
    }
};
class HoodPainter: public Painter
{
  private:
  char* ClothType;

  public:
    HoodPainter(char* ClothType, char* name) : Painter(name)
    {
      this->ClothType = ClothType;
      printf("%s", ClothType);
    }

    void Paint() override
    {
      printf("The %s is painting Hood currently\n" , this->name);
    }
  
    void Cleanup()
    {
      printf("The %s cleaning up the excess paint with %s currently\n", this->name, this->ClothType);
    }
};

int main ()
{   //Creating objects of the construct Painter
    Painter* p1 = new Painter("Ninja");
    Painter* p2 = new Painter("NinjaKul");
    Painter* p3 = new Painter("NinjaKum");
    Painter* p4 = new Painter("NinjaVal");


    DoorPainter* dp = new DoorPainter("Door Ninja");
    HoodPainter* hp = new HoodPainter("Cotton", "Hoodie Ninja");

    p1->Paint();
    p2->Paint();
    p3->Paint();
    p4->Paint();
    dp->Paint();
    hp->Paint();
    hp->Cleanup();
    return 0;
}

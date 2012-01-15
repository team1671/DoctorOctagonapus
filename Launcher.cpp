class Launcher
{
public:
    Jaguar *Top, *Bottom;
    Launcher(Jaguar *Topmotor, Jaguar *Bottommotor);
    Speedtheta(float Speed, float Theta);
};

Launcher::Launcher(Jaguar *Topmotor, Jaguar *Bottommotor)
{
    Top=Topmotor;
    Bottom=Bottommmotor;
}

Launcher::Speedtheta(float Speed, float Theta);
{
    Top->Set(Speed+Theta);
    Bottom->Set(Speed-Theta);
}

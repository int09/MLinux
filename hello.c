semaphore	DoorClose = 0;
semaphore	Stop = 0;
while (true)
{
	P(DoorClose);
	Start the car;
	Running;
	Stop the car;
	
}

while (true)
{
	Close Door;
	V(DoorClose);

	mov %al
	mov %ah
	mov %bl
	asm {
		add %cl
	}
}

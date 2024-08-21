#include "esp.hpp"

ESP::ESP(const char *driver_path, const char *name): c_driver(driver_path, this->get_pid(name))
{
    libUE4 = c_driver::getModuleBase("libUE4.so");
}

ESP::~ESP(void)
{
    if (thread) delete thread;
    puts("Release ESP done!");
}

void ESP::start(void)
{
    thread = new std::thread {[&] {
        for (; !_shutdown;)
        {
            UWorld = c_driver::read<uintptr_t>(libUE4 + 0xE9985C8);
            ULevel = c_driver::read<uintptr_t>(UWorld + 0x90);
            Array = c_driver::read<uintptr_t>(ULevel + 0xA0);
            Count = c_driver::read<int>(ULevel + 0xA8);

            if (Count < 1 || Count > 2000)
                continue;

            Matrix = c_driver::read<uintptr_t>(
                c_driver::read<uintptr_t>(
                    libUE4 + 0xE968E88
                ) + 0x20
            ) + 0x270;
            Self = c_driver::read<uintptr_t>(
                c_driver::read<uintptr_t>(
                    c_driver::read<uintptr_t>(
                        c_driver::read<uintptr_t>(
                            UWorld + 0x98
                        ) + 0x88
                    ) + 0x30
                ) + 0x2E80
            );
            my_team_id = c_driver::read<int>(Self + 0xA80);
            c_driver::read(c_driver::read<uintptr_t>(Self + 0x268) + 0x1C0, &My_pos, sizeof(struct Vec3));
            c_driver::read(Matrix, &matrix_content, sizeof(matrix_content));
            Fov = c_driver::read<float>(
                c_driver::read<uintptr_t>(
                    c_driver::read<uintptr_t>(
                        Self + 0x4A70
                    ) + 0x5D0
                ) + 0x5F0
            );

            for (int i = 0, j = 0; i < Count && j < 100; i++)
            {
                Object = c_driver::read<uintptr_t>(Array + i * sizeof(uintptr_t));

                if (Object <= 0xFFFF || Object == 0 || Object <= 0x10000000 || Object % 4 != 0 || Object >= 0x10000000000)
                    continue;

                uintptr_t object = c_driver::read<uintptr_t>(Object + 0x268);

                if (object <= 0xFFFF || object == 0 || object <= 0x10000000 || object % 4 != 0 || object >= 0x10000000000)
                    continue;

                if (c_driver::read<float>(Object + 0x2F40) != 479.5f)
                    continue;

                c_driver::read(object + 0x1C0, &players[j].Position.Position, sizeof(struct Vec3));
                if (players[j].Position.Position.x == 0.0f || players[j].Position.Position.y == 0.0f)
                    continue;

                players[j].TeamID = c_driver::read<int>(Object + 0xA80);
                if (players[j].TeamID == my_team_id)
                    continue;

                int state = c_driver::read<int>(Object + 0x1328);
                if (state == 0x100000 || state == 0x100010)
                    continue;

                players[j].Health = (c_driver::read<float>(Object + 0xDF8) / c_driver::read<float>(Object + 0xE00)) * 100;
                if (players[j].Health > 100 || players[j].Health <= 0)
                    continue;

                j++;
            }
        }
    }};
    thread->detach();
}
pid_t ESP::get_pid(const char* package)
{
    char command[69];
    int pid = -1;
    sprintf(command, "pidof %s", package);
    pid = atoi(exec(command));
    if (pid < 10)
        return -1;
    return pid;
}

const char* ESP::exec(const char* command)
{
    static char result[100 * 32];
    memset(result, '\0', sizeof(result));
    // Open pipe to file
    FILE* pipe = popen(command, "r");
    if (!pipe)
        return "popen failed!";
    // read till end of process:
    if (!fgets(result, 100 * 32, pipe))
        perror("Read command");
    pclose(pipe);
    return (const char*) result;
}

//
// Created by ryan1 on 10/02/2021.
//

#ifndef SOLIDENGINE_TASK_HPP
#define SOLIDENGINE_TASK_HPP




namespace Solid
{



    class TaskPairInterface
    {
    public:
        TaskPairInterface() =default;
        virtual void operator()() = 0;
        virtual ~TaskPairInterface() =default;

    };


    template<typename FuncType, typename... Args>
    class TaskPair : public TaskPairInterface
    {
    public:
        std::function<void()> func;


        explicit TaskPair( FuncType&& function,  Args&&... argument)
        {
            func =  std::function<void()>(std::bind(function, argument...));
        };

        TaskPair(TaskPair&& t) noexcept
        {
            func = std::move(t.func);
        }
        TaskPair(const TaskPair& pair)
        {
            func =  std::function<void(Args...)>(pair.func);
        }
        TaskPair& operator=(const TaskPair& pair)
        {
            func =  std::function<void(Args...)>(pair.func);
        }
        ~TaskPair() override = default;

        void operator()() override
        {
            if(!func)
                return ;
            func();
        }

    };



    enum class TaskType : int
    {

        RESOURCES_LOADER = 0,
        PHYSICS = 1,
        STANDALONE = 2,
        GENERAL = 3,
    };

    class Task
    {
    public:

    private:
        TaskType Type = TaskType::RESOURCES_LOADER;
        std::string ID = "NO_ID";
        bool bHasRun  = false;
        bool bDispatched = false;
        bool bInProgress = false;
        bool bExceptionCatch = false;
        struct ID_Internal
        {
            std::string ID;
        };
    public:

        std::shared_ptr<TaskPairInterface> PairData = nullptr;
        template<typename FuncType, typename... Args>
        explicit Task( FuncType&& function,  Args&&... args);
        template<typename FuncType, typename... Args>
        explicit Task(ID_Internal&& id, TaskType level, FuncType&& function, Args&&... args);

        template<typename FuncType, typename... Args>
        explicit Task( ID_Internal&& id, FuncType&& function,  Args&&... args);

        template<typename FuncType, typename... Args>
        Task* SetTask( FuncType&& function,  Args&&... args);


        Task() { PairData = nullptr;}
        Task(Task&& t) noexcept
        {
            Type = (t.Type);
            ID = std::move(t.ID);
            bHasRun = (t.bHasRun);
            bDispatched = (t.bDispatched);
            bExceptionCatch = (t.bExceptionCatch);
            PairData = t.PairData;
        }
        Task(const Task& t) = delete;
        Task& operator=(const Task& t) = delete;

        Task& operator=(Task&& t) noexcept
        {
            Type = (t.Type);
            ID = std::move(t.ID);
            bHasRun = (t.bHasRun);
            bDispatched = (t.bDispatched);
            bExceptionCatch = (t.bExceptionCatch);
            PairData = t.PairData;
            return *this;
        }

        void operator()()
        {

            try
            {

                if(PairData != nullptr)
                {
                    bInProgress = true;
                    (*PairData)();
                    bInProgress = false;
                    bHasRun = true;
                }
            }
            catch(const std::exception& e)
            {

                bExceptionCatch = true;
                bHasRun = true;
                bInProgress = false;
                std::cerr << e.what() << std::endl;
            }
            catch(const std::string_view& e)
            {

                bExceptionCatch = true;
                bHasRun = true;
                std::cerr <<"[ERROR]: Task ID " << ID <<" has encountered an exception. Message:(\"" << e <<"\")"<< std::endl;
            }

            bHasRun = true;
        }

        Task* SetType(const TaskType&& p) { Type = p; return this;}

        [[nodiscard]] TaskType GetType()  const { return Type;}
        [[nodiscard]] bool IsFinished()   const { return bHasRun;}
        [[nodiscard]] bool IsInProgress() const { return bInProgress;}
        [[nodiscard]] bool IsDispatched() const { return bDispatched;}
        Task* Dispatch()
        {
            bDispatched = true;
            return this;
        }
        Task* UnDispatch()
        {
            bDispatched = false;
            return this;
        }
        [[nodiscard]] bool Error() const { return bExceptionCatch;}
        void ResetRunning() { bHasRun = false; bExceptionCatch = false;}
        static ID_Internal MakeID(const std::string& sw) { return ID_Internal{sw};}
        std::string_view getID(){return std::forward<std::string_view>(std::string_view(ID));}
        virtual ~Task(){
            if(PairData != nullptr)
            {
                PairData = nullptr;
            }
        }

    };



    template<typename FuncType, typename... Args>
    Task::Task( FuncType&& function,  Args&&... args)
    {
        if(PairData != nullptr)
            PairData = nullptr;
        PairData = std::make_shared<TaskPair<FuncType, Args...>>(std::forward<FuncType>(function), std::forward<Args>(args)...);
        bHasRun = false;
    }
    template<typename FuncType, typename... Args>
    Task::Task(ID_Internal&& id, TaskType level, FuncType&& function, Args&&... args)
    {
        ID = id.ID;
        Type = level;
        if(PairData != nullptr)
            PairData = nullptr;
        PairData = std::make_shared<TaskPair<FuncType, Args...>>(std::forward<FuncType>(function), std::forward<Args>(args)...);
        bHasRun = false;
    }
    template<typename FuncType, typename... Args>
    Task::Task( ID_Internal&& id, FuncType&& function,  Args&&... args)
    {
        ID = id.ID;
        if(PairData != nullptr)
            PairData = nullptr;
        PairData = std::make_shared<TaskPair<FuncType, Args...>>(std::forward<FuncType>(function), std::forward<Args>(args)...);
        bHasRun = false;
    }



    template<typename FuncType, typename... Args>
    Task* Task::SetTask( FuncType&& function,  Args&&... args)
    {
        if(PairData != nullptr)
            PairData = nullptr;
        PairData = std::make_shared<TaskPair<FuncType, Args...>>(std::forward<FuncType&&>(function), std::forward<Args>(args)...);
        bHasRun = false;

        return this;

    }

}
#endif //SOLIDENGINE_TASK_HPP

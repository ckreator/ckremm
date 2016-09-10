# ckreMM - Flexible Memory Pooling System
This is a standalone project, but I need it for my future projects, e.g. for writing Embedded Systems Software (Real-time kernel, IoT Projects, Fun Projects)

I apologize for the poor structure of this project, there's no well-defined public API yet, I'll fix this later.
It's OK to use this project as module, but I'm going to use several submodules from this project for other projects.
The Header-based Memory Pool, Table-based Memory Pool, Garbage Collector, Paging Module and the Device-specific stuff will be stored in their own folders.
The bigger goal is to create an OS for mostly low-power and embedded devices which consists of many modules. ckreMM provides a few flexible memory management modules that can be included/excluded into the OS of your choice. The same goes with Processes, Scheduling, Hardware Interfaces, User-space API's.
If you want to contribute, please send me an e-mail and we'll think of a way to collaborate :)

            __                      _____      _____   
      ____ |  | _________   ____   /     \    /     \
    _/ ___\|  |/ /\_  __ \_/ __ \ /  \ /  \  /  \ /  \
    \  \___|    <  |  | \/\  ___//    Y    \/    Y    \
     \___  >__|_ \ |__|    \___  >____|__  /\____|__  /
         \/     \/             \/        \/         \/

I plan to have the following API:
Headers <Module>
- Description: Provide memory pooling through allocating a fat chunk of consequent 'Headers' that are managed by specific Headers
- Goals:
    + Provide ways to allocate, use and deallocate memory
    - Provide Multi-Processing support (syncing)
- Structures:
    - Header
    - h_pool (means pool with headers)
- Functions:
    - h_pool \*create_h_pool(size_t size);
    - void \*h_pool_alloc(poolh \*pool, size_t size);
    - bool h_pool_dealloc(h_pool \*pool, void \*ptr);
    - bool destroy_h_pool(h_pool \*pool);
    - bool clean_h_ptr(h_pool \*pool, void \*ptr);
    - bool reorder_h_mem(h_pool \*pool); // should merge all the free chunks into one big free chunk


Tables <Module>
- Description: Provide memory pooling through allocating a fat chunk of bytes and keep a separate table for managing pool segments.
- Goals:
    + Provide ways to allocate, use and deallocate memory
    + Provide a way to copy memory blocks to other blocks
    + Effectively avoid segmentation (as far as possible)
    + Provide a way to rearrange the pool and eliminate segmentation
    - Provide Multi-Processing support (syncing)
- Structures:
    - Table
    - t_pool (means pool with tables)
- Functions:
    - t_pool \*create_pool(size_t size);
    - void \*t_pool_alloc(t_pool \*pool, size_t size);
    - bool t_pool_dealloc(t_pool \*pool, void \*ptr);
    - bool destroy_t_pool(t_pool \*pool);
    - bool clean_t_ptr(t_pool \*pool, void \*ptr);
    - bool reorder_t_mem(t_pool \*pool); -> basically moves blocks around to create one large free memory block. Should lock the pool first.

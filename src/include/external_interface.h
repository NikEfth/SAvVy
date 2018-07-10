#ifndef SAVVY_INTERFACE_H
#define SAVVY_INTERFACE_H

namespace savvy_interface { 

    class ExternalInterface
    {
    public: 
        virtual ~ExternalInterface() {}
        
        virtual void load_from_workspace() = 0; 
        
        virtual void return_to_workspace() = 0; 
    };
    
};

#endif

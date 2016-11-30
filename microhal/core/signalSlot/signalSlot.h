/*
 * Slot.h
 *
 *  Created on: 13 kwi 2014
 *      Author: pawel
 */

#ifndef SLOT_H_
#define SLOT_H_

//#ifndef VENDOR_ATMEL
/* ************************************************************************************************
 * INCLUDES
 */
#include <stdlib.h>

namespace microhal {

void emptySlot(void *);
/* ************************************************************************************************
 * CLASS                     ---------------- SIGNAL ----------------
 */
template<typename ... Args>
class Signal {
    typedef void (*FunctionPtr)(void*, Args...);
    typedef struct {
        void *objectPtr;
        FunctionPtr functionPtr;

        inline void call(Args ... args) noexcept __attribute__((always_inline)) {
            if (objectPtr == nullptr) {
                (reinterpret_cast<void (*)(Args...)>(functionPtr))(args...);
            } else {
                functionPtr(objectPtr, args...);
            }
        }
    } FunctionCall;

    typedef struct {
        int count;
        FunctionCall *functionCall;
    } MultipleCall;

    FunctionCall functonCall;
public:
    Signal() noexcept:
    functonCall( {nullptr, emptySlot}) {
    }
    /**
     *
     * @param args
     */
    void emit(Args ...args) noexcept {
        functonCall.call(args...);
    }
    /**
     *
     * @param slot
     */
    void connect(void (*slot)(Args...))noexcept {
        connect((void (*)(void*, Args...))slot, nullptr);
    }
    /**
     *
     * @param slot
     * @param object
     */
    template<typename T>
    void connect(T &slot, typename T::type &object) noexcept {
        connect(slot.function, &object);
    }
    /**
     *
     * @param slot
     * @param object class object
     */
    void connect(void (*slot)(void *, Args...), void *object) noexcept {
        if(functonCall.functionPtr == emptySlot) { //none slot connected
            functonCall.functionPtr = slot;
            functonCall.objectPtr = object;
        } else if(functonCall.functionPtr == multipleEmit) {
            //2 or more slots connected
            //in functionCall.functionPtr is equal to multipleEmit
            //in functionCall.objectPtr is pointer to MultipleCall struct
            //so we need cast it:
            MultipleCall &multipleCall = *(static_cast<MultipleCall*>(functonCall.objectPtr));
            multipleCall.functionCall = static_cast<FunctionCall*>(realloc(multipleCall.functionCall,
                            (multipleCall.count + 1) * sizeof(FunctionCall)));
            //add new pointer to end
            multipleCall.functionCall[multipleCall.count].objectPtr = object;
            multipleCall.functionCall[multipleCall.count].functionPtr = slot;

            multipleCall.count++;
        } else { //if only one slot connected
            //safe actual connected object
            void *tmp = functonCall.objectPtr;

            functonCall.objectPtr = malloc(sizeof(MultipleCall));
            //in functionCall.objectPtr is pointer to MultipleCall struct so we need cast it:
            MultipleCall &multipleCall = *(static_cast<MultipleCall*>(functonCall.objectPtr));
            //we need to mallock size for 2 functon call
            multipleCall.functionCall = static_cast<FunctionCall*>(malloc(2 * sizeof(FunctionCall)));
            //in first position safe actual slot
            multipleCall.functionCall[0].objectPtr = tmp;
            multipleCall.functionCall[0].functionPtr = functonCall.functionPtr;
            //add new slot
            multipleCall.functionCall[1].objectPtr = object;
            multipleCall.functionCall[1].functionPtr = slot;
            //update slot count
            multipleCall.count = 2;

            //connect multiple emit function
            functonCall.functionPtr = multipleEmit;
        }
    }
private:
    static void multipleEmit(void *obj, Args ...args) noexcept {
        MultipleCall &multipleCall = *(static_cast<MultipleCall*>(obj));

        for(int i=0; i < multipleCall.count; i++) {
            multipleCall.functionCall[i].call(args...);
        }
    }
    static void emptySlot(void *obj __attribute__ ((unused)), Args ...args __attribute__ ((unused))) noexcept {
        microhal::emptySlot(nullptr);
    }
};
/* ************************************************************************************************
 * CLASS template specialization for void
 */
template<>
class Signal<void> {
public:
    constexpr Signal() noexcept:
    objectPtr(nullptr), functionPtr(emptySlot) {
    }
    /**
     *
     */
    void emit() noexcept {
        functionPtr(objectPtr);
    }
    /**
     *
     * @param slot
     */
    bool connect(void (*func)(void)) noexcept {
        return connect((void (*)(void*))func, nullptr);
    }
    /**
     *
     * @param slot
     * @param object
     */
    template<typename T>
    bool connect(const T &slot, const typename T::type &object) noexcept {
        return connect(slot.function, &object);
    }


    bool disconnect(void (*func)(void) = nullptr) noexcept {
    	if(functionPtr == emptySlot) return false; //no function for disconnect

    	if(functionPtr == multipleEmit) { //if some slot connected
    	}else if(functionPtr != emptySlot && (functionPtr == nullptr || functionPtr == (void (*)(void*))func)){	// if only one slot connected and all function should be disconnected or this function should be disconnected
    		functionPtr = emptySlot;
    		objectPtr = nullptr;
    		return true;
    	}
    	return false;
    }

private:
    void *objectPtr;
    void (*functionPtr)(void*);

    typedef struct {
        int count;
        struct FunctionCall {
            void *objectPtr;
            void (*functionPtr)(void*);
        }*functionCall;
    }MultipleCall;

    static void multipleEmit(void *obj) noexcept {
    	MultipleCall &funcStruct = *(static_cast<MultipleCall*>(obj));

        for(int i=0; i<funcStruct.count;i++) {
            funcStruct.functionCall[i].functionPtr(funcStruct.functionCall[i].objectPtr);
        }
    }

    /**
     *
     * @param slot
     * @param object
     */
    bool connect(void (*slot)(void*), const void *object = nullptr) noexcept {
        if (functionPtr == emptySlot) {  //none slot connected
            functionPtr = slot;
            objectPtr = const_cast<void*>(object);
        } else if (functionPtr == multipleEmit) { //2 or more slots connected
        	//2 or more slots connected
        	//in functionCall.functionPtr is equal to multipleEmit
        	//in functionCall.objectPtr is pointer to MultipleCall struct
        	//so we need cast it:
        	MultipleCall &multipleCall = *(static_cast<MultipleCall*>(objectPtr));
        	//try to allocate memory
        	void *allocated = realloc(multipleCall.functionCall, (multipleCall.count + 1) * sizeof(MultipleCall::FunctionCall));
        	if(allocated) {
        		multipleCall.functionCall = static_cast<MultipleCall::FunctionCall*>(allocated);
        		//add new pointer to end
        		multipleCall.functionCall[multipleCall.count].objectPtr = const_cast<void*>(object);
        		multipleCall.functionCall[multipleCall.count].functionPtr = slot;

        		multipleCall.count++;
        	} else {
        		return false;
        	}
        } else { //if only one slot connected
        	//safe actual connected object
			void *tmp = objectPtr;

			void *allocated = malloc(sizeof(MultipleCall));
			if(allocated){
				objectPtr = allocated;
				//in functionCall.objectPtr is pointer to MultipleCall struct so we need cast it:
				MultipleCall &multipleCall = *(static_cast<MultipleCall*>(objectPtr));
				//we need to mallock size for 2 functon call
				allocated = malloc(2 * sizeof(MultipleCall::FunctionCall));
				if (allocated){
					multipleCall.functionCall = static_cast<MultipleCall::FunctionCall*>(allocated);
					//in first position safe actual slot
					multipleCall.functionCall[0].objectPtr = tmp;
					multipleCall.functionCall[0].functionPtr = functionPtr;
					//add new slot
					multipleCall.functionCall[1].objectPtr = const_cast<void*>(object);
					multipleCall.functionCall[1].functionPtr = slot;
					//update slot count
					multipleCall.count = 2;

					//connect multiple emit function
					functionPtr = multipleEmit;
				} else {
					//free allocated memory
					free(objectPtr);
					//restore previous function call
					objectPtr = tmp;
					return false;
				}
			} else {
				return false;
			}
        }
        return true;
    }
};

/* ************************************************************************************************
 * CLASS                     ----------------- SLOT ----------------
 */
template<typename T, void (T::*func)()>
class Slot_0 {
public:
    typedef T type;

    static void function(void *obj) {
        (static_cast<T*>(obj)->*func)();
    }
};

template<typename T, typename A, void (T::*func)(A)>
class Slot_1 {
public:
    typedef T type;

    static void function(void *obj, A a) {
        (static_cast<T*>(obj)->*func)(a);
    }
};

template<typename T, typename A, typename B, void (T::*func)(A, B)>
class Slot_2 {
public:
    typedef T type;

    static void function(void *obj, A a, B b) {
        (static_cast<T*>(obj)->*func)(a, b);
    }
};

template<typename T, typename A, typename B, typename C, void (T::*func)(A, B, C)>
class Slot_3 {
public:
    typedef T type;

    static void function(void *obj, A a, B b, C c) {
        (static_cast<T*>(obj)->*func)(a, b, c);
    }
};

} // namespace microhal
//#endif
#endif // SLOT_H_

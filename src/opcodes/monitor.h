

OPCODE(monitorenter) {
	java_object_t* obj = (java_object_t*) JPOP(ptr);

	if(unlikely(!obj))
		ATHROW("java/lang/NullPointerException");


	while(obj->lock)
		avm->yield();

	obj->lock = 1;
}

OPCODE(monitorexit) {
	java_object_t* obj = (java_object_t*) JPOP(ptr);

	if(unlikely(!obj))
		ATHROW("java/lang/NullPointerException");
	

	obj->lock = 0;
}

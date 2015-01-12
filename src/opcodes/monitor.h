

OPCODE(monitorenter) {
	jobject_t* obj = (jobject_t*) JPOP(ptr);

	if(unlikely(!obj))
		j_throw(j, JEXCEPTION_NULL_POINTER);


	while(obj->lock)
		j_yield();

	obj->lock = 1;
}

OPCODE(monitorexit) {
	jobject_t* obj = (jobject_t*) JPOP(ptr);

	if(unlikely(!obj))
		j_throw(j, JEXCEPTION_NULL_POINTER);
	

	obj->lock = 0;
}

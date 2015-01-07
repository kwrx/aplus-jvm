

OPCODE(invokevirtual) {
	int16_t idx = PC16;
	PC += 2;


	methodinfo_t* method = jcode_find_methodref(j->current_assembly, idx);
	jvalue_t* params = (jvalue_t*) jmalloc(sizeof(jvalue_t) * (method->nargs + 1));

	int i = method->nargs /* + 1 (this) */;
	for(; i >= 0; i--)
		params[i] = JPOP_JV();

	R0 = jcode_method_invoke(j->current_assembly, method, params, method->nargs + 1);


#if !defined(__GLIBC__)
	jfree(params);
#endif


	jerr_check_exceptions(j);

	if(method->rettype != T_VOID)
		JPUSH_JV(R0);
}

OPCODE(invokespecial) {
	int16_t idx = PC16;
	PC += 2;


	methodinfo_t* method = jcode_find_methodref(j->current_assembly, idx);
	jvalue_t* params = (jvalue_t*) jmalloc(sizeof(jvalue_t) * (method->nargs + 1));

	int i = method->nargs /* + 1 (this) */;
	for(; i >= 0; i--)
		params[i] = JPOP_JV();

	R0 = jcode_method_invoke(j->current_assembly, method, params, method->nargs + 1);

#if !defined(__GLIBC__)
	jfree(params);
#endif


	jerr_check_exceptions(j);

	if(method->rettype != T_VOID)
		JPUSH_JV(R0);
}

OPCODE(invokestatic) {
	int16_t idx = PC16;
	PC += 2;


	methodinfo_t* method = jcode_find_methodref(j->current_assembly, idx);


	if(method->nargs == 0)
		R0 = jcode_method_invoke(j->current_assembly, method, NULL, 0);
	else {
		jvalue_t* params = (jvalue_t*) jmalloc(sizeof(jvalue_t) * method->nargs);

		int i = method->nargs - 1;
		for(; i >= 0; i--)
			params[i] = JPOP_JV();

		R0 = jcode_method_invoke(j->current_assembly, method, params, method->nargs);

#if !defined(__GLIBC__)
		jfree(params);
#endif
	}

	jerr_check_exceptions(j);

	if(method->rettype != T_VOID)
		JPUSH_JV(R0);
}

OPCODE(invokeinterface) {
	int16_t idx = PC16;
	PC += 2;

	(void) PC16;
	PC += 2;


	methodinfo_t* method = jcode_find_methodref(j->current_assembly, idx);
	jvalue_t* params = (jvalue_t*) jmalloc(sizeof(jvalue_t) * (method->nargs + 1));

	int i = method->nargs /* + 1 (this) */;
	for(; i >= 0; i--)
		params[i] = JPOP_JV();

	R0 = jcode_method_invoke(j->current_assembly, method, params, method->nargs + 1);


#if !defined(__GLIBC__)
	jfree(params);
#endif


	jerr_check_exceptions(j);

	if(method->rettype != T_VOID)
		JPUSH_JV(R0);
}

OPCODE(invokedynamic) {
	j_throw(j, JEXCEPTION_INVALID_OPCODE);
}

OPCODE(athrow) {
	jobject_t* obj = (jobject_t*) JPOP(ptr);
	JPUSH(ptr, (void*) obj);
	
	if(unlikely(!obj))
		j_throw(j, JEXCEPTION_NULL_POINTER);

	j_throw(j, obj->fullname);
}

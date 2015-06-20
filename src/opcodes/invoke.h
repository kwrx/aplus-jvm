

OPCODE(invokevirtual) {
	int16_t idx = PC16;
	PC += 2;


	java_method_t* method;
	if(java_method_find_reference(&method, j->assembly, NULL, idx) != J_OK)
		ATHROW("java/lang/NoSuchMethodError");


	j_value* params = (j_value*) avm->calloc(sizeof(j_value), (method->nargs + 1));

	int i = method->nargs /* + 1 (this) */;
	for(; i >= 0; i--)
		params[i] = JPOP_JV();


	j_value R0 = java_method_invoke(j, method->assembly, method, params, method->nargs + 1);
	avm->free(params);


	if(method->rettype != T_VOID)
		JPUSH_JV(R0);
}

OPCODE(invokespecial) {
	int16_t idx = PC16;
	PC += 2;


	java_method_t* method;
	if(java_method_find_reference(&method, j->assembly, NULL, idx) != J_OK)
		ATHROW("java/lang/NoSuchMethodError");
	

	j_value* params = (j_value*) avm->calloc(sizeof(j_value), (method->nargs + 1));

	int i = method->nargs /* + 1 (this) */;
	for(; i >= 0; i--)
		params[i] = JPOP_JV();


	j_value R0 = java_method_invoke(j, method->assembly, method, params, method->nargs + 1);
	avm->free(params);


	if(method->rettype != T_VOID)
		JPUSH_JV(R0);
}

OPCODE(invokestatic) {
	int16_t idx = PC16;
	PC += 2;


	java_method_t* method;
	if(java_method_find_reference(&method, j->assembly, NULL, idx) != J_OK)
		ATHROW("java/lang/NoSuchMethodError");


	j_value R0;

	if(method->nargs) {
		j_value* params = (j_value*) avm->calloc(sizeof(j_value), (method->nargs + 1));

		int i = method->nargs - 1;
		for(; i >= 0; i--)
			params[i] = JPOP_JV();


		R0 = java_method_invoke(j, method->assembly, method, params, method->nargs);
		avm->free(params);

	} else
		R0 = java_method_invoke(j, method->assembly, method, NULL, 0);


	if(method->rettype != T_VOID)
		JPUSH_JV(R0);
}

OPCODE(invokeinterface) {
	int16_t idx = PC16;
	PC += 2;


	java_method_t* method;
	if(java_method_find_reference(&method, j->assembly, NULL, idx) != J_OK)
		ATHROW("java/lang/NoSuchMethodError");


	j_value* params = (j_value*) avm->calloc(sizeof(j_value), (method->nargs + 1));

	int i = method->nargs /* + 1 (this) */;
	for(; i >= 0; i--)
		params[i] = JPOP_JV();


	j_value R0 = java_method_invoke(j, method->assembly, method, params, method->nargs + 1);
	avm->free(params);

	if(method->rettype != T_VOID)
		JPUSH_JV(R0);
}

OPCODE(invokedynamic) {
	ATHROW("java/lang/UnsupportedOperationException");
}

OPCODE(athrow) {
	java_object_t* obj = (java_object_t*) JPOP(ptr);
	JPUSH(ptr, (void*) obj);
	
	if(unlikely(!obj))
		ATHROW("java/lang/NullPointerException");

	athrow(j, obj->name);
}

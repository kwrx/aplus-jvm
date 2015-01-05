
package java.lang;

public final class Object {

	protected Object clone() {
		return jvm.Runtime.clone(this);
	}

	protected void finalize() {
		jvm.Runtime.finalize(this);
	}

	public boolean equals(Object obj) {
		return (this == obj);
	}

	public String toString() {
		return "java/lang/Object";
	}

}

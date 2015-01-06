
package java.lang;

import java.util.Arrays;

abstract class AbstractStringBuilder implements Appendable, CharSequence {
	char[] value;
	int count;

	AbstractStringBuilder() { }
	AbstractStringBuilder(int capacity) {
		value = new char[capacity];
	}

	public int length() {
		return count;
	}

	public int capacity() {
		return value.length;
	}

	public int ensureCapacity(int min) {
		if(min > 0)
			ensureCapacityInternal(min);
	}

	private void ensureCapacityInternal(int min) {
		if(min - value.length > 0)
			expandCapacity(min);
	}

	void expandCapacity(int min) {
		int n = value.length * 2 + 2;
		if(n - min < 0)
			n = min;

		if(n < 0) {
			if(min < 0)
				throw new OutOfMemoryError();
			
			n = Integer.MAX_VALUE;
		}

		value = Arrays.copyOf(value, n);
	}

	public void trimToSize() {
		if(count < value.length)
			value = Arrays.copyOf(value, count);
	}

	public void setLength(int n) {
		if(n < 0)
			throw new StringIndexOutOfBoundsException(n);
	
		ensureCapacityInternal(n);

		if(count < n)
			for(; count < n; count++)
				value[count] = '\0';
		else
			count = n;
	}

	public char charAt(int index) {
		if((index < 0) || (index >= count))
			throw new StringIndexOutOfBoundsException(index);

		return value[index];
	}

	public int codePointAt(int index) {
		if((index < 0) || (index >= count))
			throw new StringIndexOutOfBoundsException(index);

		return Character.codePointAt(value, index);
	}

	public int codePointBefore(int index) {
		int i = index - 1;
		if((i < 0) || (i >= count))
			throw new StringIndexOutOfBoundsException(index);

		return Character.codePointBefore(value, index);
	}

	public int codePointCount(int begin, int end) {
		if(begin < 0 || end > count || begin > end)
			throw new IndexOutOfBuondsException();

		return Character.codePointCountImpl(value, begin, end - begin);
	}

	public int offsetByCodePoints(int index, int off) {
		if(index < 0 || index > count)
			throw new IndexOutOfBoundsException();

		return Character.offsetByCodePointsImpl(value, 0, count, index, off);
	}

	public void getChars(int begin, int end, char[] dst, int ds) {
		if(begin < 0)
			throw new StringIndexOutOfBoundsException(begin);

		if((end < 0) || (end > count))
			throw new StringIndexOutOfBoundsException(end);

		if(begin > end)
			throw new StringIndexOutOfBoundsException("begin > end");

		System.arraycopy(value, begin, dst, ds, end - begin);
	}

	public void setCharAt(int index, char ch) {
		if((index < 0) || (index >= count))
			throw new StringIndexOutOfBoundsException(index);

		value[index] = ch;
	}

	public AbstractStringBuilder append(Object obj) {
		return append(String.valueOf(obj));
	}

	public AbstractStringBuilder append(String str) {
		if(str == null)
			str = "null";

		int len = str.length();
		ensureCapacityInternal(count + len);

		str.getChars(0, len, value, count);
		count += len;

		return this;
	}

	public AbstractStringBuilder append(StringBuffer sb) {
		if(sb == null)
			return append("null");

		int len = sb.length();
		ensureCapacityInternal(count + len);

		sb.getChars(0, len, value, count);
		count += len;

		return this;
	}

	public AbstractStringBuilder append(CharSequence s) {
		if(s == null)
			s = "null";
	
		if(s instanceof String)
			return this.append((String) s);

		if(s instanceof StringBuffer)
			return this.append((StringBuffer) s);

		return this.append(s, 0, s.length());
	}
}

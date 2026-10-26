// NativeResources.java - звільнення нативної пам'яті разом із GC
// Author: Андрій Будильников (Sparky)

package com.cton.sdk;

import java.lang.ref.Cleaner;
import java.util.concurrent.atomic.AtomicBoolean;

import com.sun.jna.Pointer;

/**
 * Реєструє нативний вказівник у {@link Cleaner}.
 * Дія прибирання тримає лише адресу і деструктор, без посилання на Java-об'єкт,
 * інакше об'єкт ніколи не стане фантомно досяжним.
 */
final class NativeResources {
    private static final Cleaner CLEANER = Cleaner.create();

    interface Disposer {
        void dispose(Pointer pointer);
    }

    private NativeResources() {
    }

    static Cleaner.Cleanable track(Object owner, Pointer pointer, Disposer disposer) {
        if (pointer == null) {
            return null;
        }
        long peer = Pointer.nativeValue(pointer);
        return CLEANER.register(owner, new Free(peer, disposer));
    }

    private static final class Free implements Runnable {
        private final AtomicBoolean done = new AtomicBoolean(false);
        private final long peer;
        private final Disposer disposer;

        Free(long peer, Disposer disposer) {
            this.peer = peer;
            this.disposer = disposer;
        }

        @Override
        public void run() {
            if (peer == 0 || !done.compareAndSet(false, true)) {
                return;
            }
            disposer.dispose(new Pointer(peer));
        }
    }
}

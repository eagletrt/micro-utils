FROM gcc:14.2.0

# Install cmake (and build-essential extras if needed)
RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake \
    && rm -rf /var/lib/apt/lists/*

# Build-time args for matching host user
ARG USER_ID=1000
ARG GROUP_ID=1000
ARG USER_NAME=pi

# Create a group/user matching the host, reusing existing IDs if they collide
RUN if getent group ${GROUP_ID} >/dev/null; then \
        groupmod -n ${USER_NAME} $(getent group ${GROUP_ID} | cut -d: -f1); \
    else \
        groupadd -g ${GROUP_ID} ${USER_NAME}; \
    fi && \
    if getent passwd ${USER_ID} >/dev/null; then \
        usermod -l ${USER_NAME} -d /home/${USER_NAME} -m $(getent passwd ${USER_ID} | cut -d: -f1); \
    else \
        useradd -u ${USER_ID} -g ${GROUP_ID} -m ${USER_NAME}; \
    fi

USER ${USER_NAME}
WORKDIR /home/${USER_NAME}/custom-message-sender
